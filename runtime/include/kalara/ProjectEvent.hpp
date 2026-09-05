#pragma once

#include "kalara/UUID.hpp"
#include "kalara/Json.hpp"
#include "kalara/Entity.hpp"
#include <string>
#include <cstdint>

namespace kalara::runtime {

enum class ActorType : uint8_t {
    Human = 0,
    AI = 1,
    System = 2
};

struct ProjectEvent {
    core::UUID event_id{};
    core::UUID transaction_id{};
    uint64_t timestamp{0};
    ActorType actor_type{ActorType::Human};
    std::string actor_name{"User"};
    std::string action_type{"UnknownAction"};
    EntityID target_entity_id{NullEntityID};
    core::JsonValue before_state{};
    core::JsonValue after_state{};
    std::string description{};
};

} // namespace kalara::runtime
