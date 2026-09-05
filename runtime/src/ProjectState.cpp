#include "kalara/ProjectState.hpp"
#include "kalara/Component.hpp"
#include "kalara/Log.hpp"
#include <chrono>

namespace kalara::runtime {

static uint64_t current_time_millis() {
    return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count());
}

ProjectState& ProjectState::instance() noexcept {
    static ProjectState s_instance;
    return s_instance;
}

core::UUID ProjectState::begin_transaction(ActorType actor, std::string_view actor_name) noexcept {
    (void)actor;
    (void)actor_name;
    m_active_transaction_id = core::UUID::generate();
    return m_active_transaction_id;
}

void ProjectState::commit_transaction(const core::UUID& tx_id) {
    if (m_active_transaction_id == tx_id) {
        m_active_transaction_id = core::UUID{};
    }
}

void ProjectState::record_event(ProjectEvent event) {
    if (event.event_id == core::UUID{}) {
        event.event_id = core::UUID::generate();
    }
    if (event.transaction_id == core::UUID{} && m_active_transaction_id != core::UUID{}) {
        event.transaction_id = m_active_transaction_id;
    }
    if (event.timestamp == 0) {
        event.timestamp = current_time_millis();
    }

    KALARA_LOG_INFO("[ProjectEvent] Actor: '{}', Action: '{}', Target: {}",
                    event.actor_name, event.action_type, event.target_entity_id);
    m_events.push_back(std::move(event));
}

bool ProjectState::rollback_event(const ProjectEvent& event, Scene& scene) {
    if (event.action_type == "CreateEntity") {
        if (scene.registry().valid(event.target_entity_id)) {
            scene.destroy_entity(event.target_entity_id);
            return true;
        }
    } else if (event.action_type == "ModifyTransform") {
        if (scene.registry().valid(event.target_entity_id) &&
            scene.registry().has_component<TransformComponent>(event.target_entity_id)) {
            auto& t = scene.registry().get_component<TransformComponent>(event.target_entity_id);
            if (event.before_state.contains("px")) {
                t.position.x = static_cast<float>(event.before_state["px"].as_number());
                t.position.y = static_cast<float>(event.before_state["py"].as_number());
                t.position.z = static_cast<float>(event.before_state["pz"].as_number());
                return true;
            }
        }
    }
    return false;
}

void ProjectState::add_task(std::string_view title, std::string_view description) {
    ProjectTask task;
    task.id = core::UUID::generate();
    task.title = std::string(title);
    task.description = std::string(description);
    task.is_completed = false;
    m_tasks.push_back(task);
}

void ProjectState::complete_task(const core::UUID& task_id) {
    for (auto& task : m_tasks) {
        if (task.id == task_id) {
            task.is_completed = true;
            break;
        }
    }
}

void ProjectState::record_decision(std::string_view title, std::string_view rationale) {
    ProjectDecision decision;
    decision.id = core::UUID::generate();
    decision.title = std::string(title);
    decision.rationale = std::string(rationale);
    decision.timestamp = current_time_millis();
    m_decisions.push_back(decision);
}

void ProjectState::log_error(std::string_view error_message) {
    ProjectError err;
    err.id = core::UUID::generate();
    err.message = std::string(error_message);
    err.timestamp = current_time_millis();
    m_errors.push_back(err);
}

ProjectSummary ProjectState::get_summary(const Scene* active_scene) const noexcept {
    ProjectSummary summary;
    summary.project_name = "KaLara Project";
    summary.scene_count = 1;
    summary.entity_count = active_scene ? active_scene->registry().entity_count() : 0;

    size_t pending = 0;
    for (const auto& task : m_tasks) {
        if (!task.is_completed) pending++;
    }
    summary.pending_tasks = pending;
    summary.recorded_events = m_events.size();

    return summary;
}

} // namespace kalara::runtime
