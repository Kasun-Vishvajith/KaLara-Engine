#include "kalara/UUID.hpp"
#include <random>

namespace kalara::core {

static std::random_device s_random_device;
static std::mt19937_64 s_engine(s_random_device());
static std::uniform_int_distribution<uint64_t> s_distribution;

AssetID UUID::generate() noexcept {
    AssetID id = s_distribution(s_engine);
    while (id == 0) { // 0 represents invalid/null AssetID
        id = s_distribution(s_engine);
    }
    return id;
}

} // namespace kalara::core
