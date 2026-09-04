cmake_minimum_required(VERSION 3.25)

project(KaLara
    VERSION 0.0.0
    DESCRIPTION "KaLara — human-first, AI-native 2D game engine"
    LANGUAGES CXX
)

# KaLara is intentionally tiny at Step 00. The first milestone only proves
# that the repository/toolchain can configure and build safely.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(KALARA_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

if(MSVC)
    add_compile_options(/W4 /permissive-)
    if(KALARA_WARNINGS_AS_ERRORS)
        add_compile_options(/WX)
    endif()
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    if(KALARA_WARNINGS_AS_ERRORS)
        add_compile_options(-Werror)
    endif()
endif()

# Keep the initial target structure explicit. Each layer will become its own
# library as implementation begins.
add_library(KaLaraCore STATIC
    core/src/Core.cpp
)

target_include_directories(KaLaraCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/core/include
)

add_library(KaLaraRuntime STATIC
    engine/src/Runtime.cpp
)

target_include_directories(KaLaraRuntime
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/include
)

target_link_libraries(KaLaraRuntime
    PUBLIC KaLaraCore
)

add_executable(KaLaraEditor
    editor/src/main.cpp
)

target_link_libraries(KaLaraEditor
    PRIVATE KaLaraRuntime
)

# Step 00 test executable. Keep tests independent of future engine systems.
add_executable(KaLaraSmokeTest
    tests/smoke/main.cpp
)

target_link_libraries(KaLaraSmokeTest
    PRIVATE KaLaraCore
)

enable_testing()
add_test(NAME KaLara.Smoke COMMAND KaLaraSmokeTest)

# Safe output layout for multi-config generators such as Visual Studio.
set_target_properties(KaLaraEditor KaLaraSmokeTest PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>"
)
