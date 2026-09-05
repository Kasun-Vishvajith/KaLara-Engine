#include "kalara/UndoRedoManager.hpp"
#include "kalara/Scene.hpp"
#include "kalara/Component.hpp"
#include <cassert>

int main() {
    auto& undo_mgr = kalara::runtime::UndoRedoManager::instance();
    undo_mgr.clear();

    auto scene = kalara::runtime::Scene::create("Undo Test Scene");

    // 1. Test Transaction Grouping & Action Recording
    undo_mgr.begin_transaction(kalara::runtime::ActorType::Human, "Create Player Hero");

    kalara::runtime::EntityID player = scene->create_entity("PlayerHero");
    auto& t = scene->registry().get_component<kalara::runtime::TransformComponent>(player);

    kalara::runtime::ProjectEvent event1;
    event1.action_type = "CreateEntity";
    event1.actor_type = kalara::runtime::ActorType::Human;
    event1.target_entity_id = player;
    undo_mgr.record_event(event1);

    kalara::runtime::ProjectEvent event2;
    event2.action_type = "ModifyTransform";
    event2.actor_type = kalara::runtime::ActorType::Human;
    event2.target_entity_id = player;
    event2.before_state["px"] = 0.0;
    event2.before_state["py"] = 0.0;
    event2.before_state["pz"] = 0.0;
    event2.after_state["px"] = 500.0;
    event2.after_state["py"] = 300.0;
    event2.after_state["pz"] = 0.0;

    t.position = {500.0f, 300.0f, 0.0f};
    undo_mgr.record_event(event2);

    undo_mgr.commit_transaction();

    assert(undo_mgr.can_undo());
    assert(undo_mgr.undo_count() == 1);
    assert(scene->registry().entity_count() == 1);
    assert(t.position.x == 500.0f);

    // 2. Test Undo Execution
    bool undo_ok = undo_mgr.undo(*scene);
    assert(undo_ok);
    assert(undo_mgr.can_redo());
    assert(undo_mgr.redo_count() == 1);
    assert(scene->registry().entity_count() == 0); // Entity destroyed by undoing CreateEntity

    // 3. Test Redo Execution
    bool redo_ok = undo_mgr.redo(*scene);
    assert(redo_ok);
    assert(scene->registry().entity_count() == 1); // Entity re-created by redo

    undo_mgr.clear();
    return 0;
}
