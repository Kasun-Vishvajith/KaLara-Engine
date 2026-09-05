#include "kalara/UndoRedoManager.hpp"
#include "kalara/ProjectState.hpp"
#include "kalara/Component.hpp"
#include "kalara/Log.hpp"

namespace kalara::runtime {

UndoRedoManager& UndoRedoManager::instance() noexcept {
    static UndoRedoManager s_instance;
    return s_instance;
}

void UndoRedoManager::begin_transaction(ActorType actor, std::string_view name) noexcept {
    m_active_transaction = TransactionRecord{};
    m_active_transaction.id = core::UUID::generate();
    m_active_transaction.actor_type = actor;
    m_active_transaction.name = std::string(name);
    m_in_transaction = true;
}

void UndoRedoManager::record_event(ProjectEvent event) {
    if (m_in_transaction) {
        event.transaction_id = m_active_transaction.id;
        m_active_transaction.events.push_back(event);
    } else {
        TransactionRecord single_tx;
        single_tx.id = core::UUID::generate();
        single_tx.actor_type = event.actor_type;
        single_tx.name = event.action_type;
        single_tx.events.push_back(event);
        m_undo_stack.push_back(std::move(single_tx));
        m_redo_stack.clear();
    }
    ProjectState::instance().record_event(event);
}

void UndoRedoManager::commit_transaction() {
    if (m_in_transaction) {
        if (!m_active_transaction.events.empty()) {
            m_undo_stack.push_back(m_active_transaction);
            m_redo_stack.clear();
        }
        m_active_transaction = TransactionRecord{};
        m_in_transaction = false;
    }
}

bool UndoRedoManager::undo(Scene& scene) {
    if (!can_undo()) return false;

    TransactionRecord tx = std::move(m_undo_stack.back());
    m_undo_stack.pop_back();

    KALARA_LOG_INFO("[Undo] Reverting transaction '{}' ({} events)", tx.name, tx.events.size());

    // Rollback events in reverse order
    for (auto it = tx.events.rbegin(); it != tx.events.rend(); ++it) {
        ProjectState::instance().rollback_event(*it, scene);
    }

    m_redo_stack.push_back(std::move(tx));
    return true;
}

bool UndoRedoManager::redo(Scene& scene) {
    if (!can_redo()) return false;

    TransactionRecord tx = std::move(m_redo_stack.back());
    m_redo_stack.pop_back();

    KALARA_LOG_INFO("[Redo] Re-applying transaction '{}' ({} events)", tx.name, tx.events.size());

    // Apply events in forward order
    for (const auto& event : tx.events) {
        apply_event(event, scene);
    }

    m_undo_stack.push_back(std::move(tx));
    return true;
}

bool UndoRedoManager::apply_event(const ProjectEvent& event, Scene& scene) {
    if (event.action_type == "CreateEntity") {
        EntityID e = scene.create_entity("RestoredEntity");
        (void)e;
        return true;
    } else if (event.action_type == "ModifyTransform") {
        if (scene.registry().valid(event.target_entity_id) &&
            scene.registry().has_component<TransformComponent>(event.target_entity_id)) {
            auto& t = scene.registry().get_component<TransformComponent>(event.target_entity_id);
            if (event.after_state.contains("px")) {
                t.position.x = static_cast<float>(event.after_state["px"].as_number());
                t.position.y = static_cast<float>(event.after_state["py"].as_number());
                t.position.z = static_cast<float>(event.after_state["pz"].as_number());
                return true;
            }
        }
    }
    return false;
}

} // namespace kalara::runtime
