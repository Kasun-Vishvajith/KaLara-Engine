#pragma once

#include <cstdint>
#include <string>

namespace kalara::core {

using AssetID = uint64_t;

class UUID {
public:
    static AssetID generate() noexcept;
};

} // namespace kalara::core
