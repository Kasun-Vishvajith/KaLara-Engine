#pragma once

#include "kalara/Registry.hpp"

namespace kalara::runtime {

class BehaviorSystem {
public:
    static void update(Registry& registry, float delta_time);
};

} // namespace kalara::runtime
