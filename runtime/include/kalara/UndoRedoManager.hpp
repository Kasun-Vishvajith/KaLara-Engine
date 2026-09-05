#pragma once

#include "kalara/ProjectEvent.hpp"
#include "kalara/Scene.hpp"
#include <vector>
#include <string>
#include <string_view>

namespace kalara::runtime {

struct TransactionRecord {
    core::UUID id{};
    ActorType actor_type{ActorType::Human};
    std::string name{"Transaction"};
    std::vector<ProjectEvent> events{};
};

class UndoRedoManager {
public:
    UndoRedoManager() = default;
    ~UndoRedoManager() = default;

    static UndoRedoManager& instance() noexcept;

    void begin_transaction(ActorType actor = ActorType::Human, std::string_view name = "Operation") noexcept;
    void record_event(ProjectEvent event);
    void commit_transaction();

    bool undo(Scene& scene);
    bool redo(Scene& scene);

    [[nodiscard]] bool can_undo() const noexcept { return !m_undo_stack.empty(); }
    [[nodiscard]] bool can_redo() const noexcept { return !m_redo_stack.empty(); }
    [[nodiscard]] size_t undo_count() const noexcept { return m_undo_stack.size(); }
    [[nodiscard]] size_t redo_count() const noexcept { return m_redo_stack.size(); }

    void clear() noexcept {
        m_undo_stack.clear();
        m_redo_stack.clear();
        m_active_transaction = TransactionRecord{};
        m_in_transaction = false;
    }

private:
    bool apply_event(const ProjectEvent& event, Scene& scene);

    std::vector<TransactionRecord> m_undo_stack;
    std::vector<TransactionRecord> m_redo_stack;
    TransactionRecord m_active_transaction;
    bool m_in_transaction{false};
};

} // namespace kalara::runtime
