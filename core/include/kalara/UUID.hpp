#pragma once

#include <cstdint>
#include <string>

namespace kalara::core {

using AssetID = uint64_t;
using EntityID = uint64_t;

class UUID {
public:
    static uint64_t generate() noexcept;
    static std::string format_entity_id(EntityID id);
};

} // namespace kalara::core
