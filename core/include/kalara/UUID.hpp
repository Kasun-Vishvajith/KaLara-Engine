#pragma once

#include <cstdint>
#include <string>

namespace kalara::core {

using AssetID = uint64_t;
using EntityID = uint64_t;

class UUID {
public:
    constexpr UUID() noexcept : m_id(0) {}
    constexpr UUID(uint64_t id) noexcept : m_id(id) {}

    static UUID generate() noexcept;

    [[nodiscard]] constexpr uint64_t raw() const noexcept { return m_id; }
    [[nodiscard]] std::string to_string() const { return std::to_string(m_id); }

    constexpr operator uint64_t() const noexcept { return m_id; }

    constexpr bool operator==(const UUID& other) const noexcept { return m_id == other.m_id; }
    constexpr bool operator!=(const UUID& other) const noexcept { return m_id != other.m_id; }
    constexpr bool operator<(const UUID& other) const noexcept { return m_id < other.m_id; }

    static std::string format_entity_id(EntityID id) { return "entity_" + std::to_string(id); }

private:
    uint64_t m_id{0};
};

} // namespace kalara::core
