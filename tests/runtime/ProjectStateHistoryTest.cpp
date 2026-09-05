#include "kalara/ProjectState.hpp"
#include "kalara/Scene.hpp"
#include "kalara/Component.hpp"
#include <cassert>

int main() {
    auto& state = kalara::runtime::ProjectState::instance();
    auto scene = kalara::runtime::Scene::create("History Test Scene");

    // 1. Test Transaction & Event Recording
    kalara::core::UUID tx = state.begin_transaction(kalara::runtime::ActorType::AI, "AI Coding Agent");

    kalara::runtime::EntityID entity = scene->create_entity("BoxHero");
    auto& t = scene->registry().get_component<kalara::runtime::TransformComponent>(entity);
    t.position = {10.0f, 20.0f, 0.0f};

    kalara::runtime::ProjectEvent event;
    event.action_type = "CreateEntity";
    event.actor_type = kalara::runtime::ActorType::AI;
    event.actor_name = "AI Agent";
    event.target_entity_id = entity;
    event.description = "AI created entity BoxHero";

    state.record_event(event);
    state.commit_transaction(tx);

    assert(state.history().size() >= 1);
    assert(state.history().back().action_type == "CreateEntity");

    // 2. Test Task & Decision Tracking
    state.add_task("Add Physics Collider", "Attach BoxCollider2D to BoxHero");
    assert(state.tasks().size() == 1);
    assert(!state.tasks()[0].is_completed);

    state.complete_task(state.tasks()[0].id);
    assert(state.tasks()[0].is_completed);

    state.record_decision("Use Box2D Engine", "Approved lightweight 2D rigid body engine for V1");
    assert(state.decisions().size() == 1);

    state.log_error("Sample diagnostic error log");
    assert(state.errors().size() == 1);

    // 3. Test Project Summary Generation
    auto summary = state.get_summary(scene.get());
    assert(summary.entity_count == 1);
    assert(summary.pending_tasks == 0);
    assert(summary.recorded_events >= 1);

    // 4. Test Event Rollback (CreateEntity rollback should destroy the entity)
    bool rolled_back = state.rollback_event(event, *scene);
    assert(rolled_back);
    assert(scene->registry().entity_count() == 0);

    return 0;
}
