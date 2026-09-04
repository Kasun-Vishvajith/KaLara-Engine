cmake_minimum_required(VERSION 3.25)

project(KaLara
    VERSION 0.1.0
    DESCRIPTION "KaLara — human-first, AI-native 2D game engine"
    LANGUAGES CXX
)

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

# Core layer static library
add_library(KaLaraCore STATIC
    core/src/Core.cpp
    core/src/Log.cpp
    core/src/Config.cpp
)

target_include_directories(KaLaraCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/core/include
)

# Runtime layer static library
add_library(KaLaraRuntime STATIC
    runtime/src/Runtime.cpp
)

target_include_directories(KaLaraRuntime
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/runtime/include
)

target_link_libraries(KaLaraRuntime
    PUBLIC KaLaraCore
)

# Editor layer executable target
add_executable(KaLaraEditor
    editor/src/main.cpp
)

target_link_libraries(KaLaraEditor
    PRIVATE KaLaraRuntime
)

# Unit test targets
add_executable(KaLaraCoreTest
    tests/core/CoreTest.cpp
)

target_link_libraries(KaLaraCoreTest
    PRIVATE KaLaraCore
)

add_executable(KaLaraSmokeTest
    tests/smoke/main.cpp
)

target_link_libraries(KaLaraSmokeTest
    PRIVATE KaLaraRuntime
)

enable_testing()
add_test(NAME KaLara.Core COMMAND KaLaraCoreTest)
add_test(NAME KaLara.Smoke COMMAND KaLaraSmokeTest)

# Safe output layout for build artifacts
set_target_properties(KaLaraEditor KaLaraCoreTest KaLaraSmokeTest PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>"
)
