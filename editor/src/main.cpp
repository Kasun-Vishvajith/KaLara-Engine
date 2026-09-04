#include <iostream>

#include "kalara/Core.hpp"
#include "kalara/Runtime.hpp"

int main() {
    std::cout << "KaLara development foundation " << kalara::core::version() << '\n';
    std::cout << "Runtime foundation status: " << kalara::runtime::foundation_status() << '\n';
    return 0;
}
