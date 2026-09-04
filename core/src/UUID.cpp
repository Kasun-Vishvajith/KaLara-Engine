#include "kalara/UUID.hpp"
#include <random>
#include <sstream>
#include <iomanip>

namespace kalara::core {

static std::random_device s_random_device;
static std::mt19937_64 s_engine(s_random_device());
static std::uniform_int_distribution<uint64_t> s_distribution;

uint64_t UUID::generate() noexcept {
    uint64_t id = s_distribution(s_engine);
    while (id == 0) {
        id = s_distribution(s_engine);
    }
    return id;
}

std::string UUID::format_entity_id(EntityID id) {
    std::stringstream ss;
    ss << "entity_" << std::hex << std::setw(6) << std::setfill('0') << (id & 0xFFFFFF);
    return ss.str();
}

} // namespace kalara::core
