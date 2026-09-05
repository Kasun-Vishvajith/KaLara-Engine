#pragma once

#include "kalara/ProjectEvent.hpp"
#include "kalara/Scene.hpp"
#include <string>
#include <vector>
#include <memory>

namespace kalara::runtime {

struct ProjectTask {
    core::UUID id{};
    std::string title{};
    std::string description{};
    bool is_completed{false};
};

struct ProjectDecision {
    core::UUID id{};
    std::string title{};
    std::string rationale{};
    uint64_t timestamp{0};
};

struct ProjectError {
    core::UUID id{};
    std::string message{};
    uint64_t timestamp{0};
};

struct ProjectSummary {
    std::string project_name{"KaLara Project"};
    size_t scene_count{0};
    size_t entity_count{0};
    size_t pending_tasks{0};
    size_t recorded_events{0};
};

class ProjectState {
public:
    ProjectState() = default;
    ~ProjectState() = default;

    static ProjectState& instance() noexcept;

    core::UUID begin_transaction(ActorType actor = ActorType::Human, std::string_view actor_name = "User") noexcept;
    void commit_transaction(const core::UUID& tx_id);

    void record_event(ProjectEvent event);
    bool rollback_event(const ProjectEvent& event, Scene& scene);

    void add_task(std::string_view title, std::string_view description);
    void complete_task(const core::UUID& task_id);

    void record_decision(std::string_view title, std::string_view rationale);
    void log_error(std::string_view error_message);

    [[nodiscard]] ProjectSummary get_summary(const Scene* active_scene = nullptr) const noexcept;
    [[nodiscard]] const std::vector<ProjectEvent>& history() const noexcept { return m_events; }
    [[nodiscard]] const std::vector<ProjectTask>& tasks() const noexcept { return m_tasks; }
    [[nodiscard]] const std::vector<ProjectDecision>& decisions() const noexcept { return m_decisions; }
    [[nodiscard]] const std::vector<ProjectError>& errors() const noexcept { return m_errors; }

private:
    std::vector<ProjectEvent> m_events;
    std::vector<ProjectTask> m_tasks;
    std::vector<ProjectDecision> m_decisions;
    std::vector<ProjectError> m_errors;
    core::UUID m_active_transaction_id{};
};

} // namespace kalara::runtime
