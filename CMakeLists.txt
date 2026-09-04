cmake_minimum_required(VERSION 3.25)

project(KaLara
    VERSION 0.10.0
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

# Fetch SDL3 dependency
include(FetchContent)
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.8
    GIT_SHALLOW TRUE
)
set(SDL_SHARED OFF CACHE BOOL "Build SDL shared library" FORCE)
set(SDL_STATIC ON CACHE BOOL "Build SDL static library" FORCE)
set(SDL_TEST OFF CACHE BOOL "Build SDL tests" FORCE)
FetchContent_MakeAvailable(SDL3)

find_package(OpenGL REQUIRED)

# Core layer static library
add_library(KaLaraCore STATIC
    core/src/Core.cpp
    core/src/Log.cpp
    core/src/Config.cpp
    core/src/UUID.cpp
    core/src/Json.cpp
)

target_include_directories(KaLaraCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/core/include
)

# Runtime layer static library
add_library(KaLaraRuntime STATIC
    runtime/src/Runtime.cpp
    runtime/src/Window.cpp
    runtime/src/Renderer.cpp
    runtime/src/Buffer.cpp
    runtime/src/VertexArray.cpp
    runtime/src/Shader.cpp
    runtime/src/Image.cpp
    runtime/src/Texture.cpp
    runtime/src/AssetManager.cpp
    runtime/src/Registry.cpp
    runtime/src/Hierarchy.cpp
    runtime/src/Scene.cpp
    runtime/src/SceneSerializer.cpp
    runtime/src/Input.cpp
    runtime/src/Camera2D.cpp
    runtime/src/PhysicsWorld2D.cpp
    runtime/src/SpriteSheet.cpp
    runtime/src/Animation.cpp
)

target_include_directories(KaLaraRuntime
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/runtime/include
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

target_link_libraries(KaLaraRuntime
    PUBLIC
        KaLaraCore
        SDL3::SDL3
        OpenGL::GL
)

# Editor layer static library
add_library(KaLaraEditorLib STATIC
    editor/src/EditorState.cpp
    editor/src/EditorHierarchyPanel.cpp
    editor/src/EditorInspectorPanel.cpp
    editor/src/EditorViewportPanel.cpp
    editor/src/EditorAssetBrowserPanel.cpp
)

target_include_directories(KaLaraEditorLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/editor/include
)

target_link_libraries(KaLaraEditorLib
    PUBLIC KaLaraRuntime
)

# Editor layer executable target
add_executable(KaLaraEditor
    editor/src/main.cpp
)

target_link_libraries(KaLaraEditor
    PRIVATE KaLaraEditorLib
)

# Unit test targets
add_executable(KaLaraCoreTest
    tests/core/CoreTest.cpp
)

target_link_libraries(KaLaraCoreTest
    PRIVATE KaLaraCore
)

add_executable(KaLaraRendererTest
    tests/runtime/RendererTest.cpp
)

target_link_libraries(KaLaraRendererTest
    PRIVATE KaLaraRuntime
)

add_executable(KaLaraAssetTest
    tests/runtime/AssetTest.cpp
)

target_link_libraries(KaLaraAssetTest
    PRIVATE KaLaraRuntime
)

add_executable(KaLaraEntityTest
    tests/runtime/EntityTest.cpp
)

target_link_libraries(KaLaraEntityTest
    PRIVATE KaLaraRuntime
)

add_executable(KaLaraSceneTest
    tests/runtime/SceneSerializationTest.cpp
)

target_link_libraries(KaLaraSceneTest
    PRIVATE KaLaraRuntime
)

add_executable(KaLaraInputCameraTest
    tests/runtime/InputCameraTest.cpp
)

target_link_libraries(KaLaraInputCameraTest
    PRIVATE KaLaraRuntime
)

add_executable(KaLaraPhysicsTest
    tests/runtime/PhysicsTest.cpp
)

target_link_libraries(KaLaraPhysicsTest
    PRIVATE KaLaraRuntime
)

add_executable(KaLaraAnimationTest
    tests/runtime/AnimationTest.cpp
)

target_link_libraries(KaLaraAnimationTest
    PRIVATE KaLaraRuntime
)

add_executable(KaLaraEditorTest
    tests/editor/EditorFoundationTest.cpp
)

target_link_libraries(KaLaraEditorTest
    PRIVATE KaLaraEditorLib
)

add_executable(KaLaraSmokeTest
    tests/smoke/main.cpp
)

target_link_libraries(KaLaraSmokeTest
    PRIVATE KaLaraRuntime
)

enable_testing()
add_test(NAME KaLara.Core COMMAND KaLaraCoreTest)
add_test(NAME KaLara.Renderer COMMAND KaLaraRendererTest)
add_test(NAME KaLara.Asset COMMAND KaLaraAssetTest)
add_test(NAME KaLara.Entity COMMAND KaLaraEntityTest)
add_test(NAME KaLara.Scene COMMAND KaLaraSceneTest)
add_test(NAME KaLara.InputCamera COMMAND KaLaraInputCameraTest)
add_test(NAME KaLara.Physics COMMAND KaLaraPhysicsTest)
add_test(NAME KaLara.Animation COMMAND KaLaraAnimationTest)
add_test(NAME KaLara.Editor COMMAND KaLaraEditorTest)
add_test(NAME KaLara.Smoke COMMAND KaLaraSmokeTest)

# Safe output layout for build artifacts
set_target_properties(KaLaraEditor KaLaraCoreTest KaLaraRendererTest KaLaraAssetTest KaLaraEntityTest KaLaraSceneTest KaLaraInputCameraTest KaLaraPhysicsTest KaLaraAnimationTest KaLaraEditorTest KaLaraSmokeTest PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>"
)
