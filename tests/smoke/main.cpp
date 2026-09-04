#include "kalara/Core.hpp"

int main() {
    return kalara::core::version() == "0.0.0" ? 0 : 1;
}
