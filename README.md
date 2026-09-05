# KaLara

**KaLara** is an open-source, human-first, AI-native 2D game engine.

> **Current status:** Step 11 complete — Semantic System implemented.  
> **Next step:** Step 12 — C# Game Layer.

---

## Vision

KaLara's core goal:

> Make it possible for **beginners to create real games without programming**, while providing external AI coding agents with a structured, machine-readable way to understand and modify the same project.

KaLara works completely **without AI**. AI is an optional external development assistant — the engine contains no embedded model, no cloud dependency, and no LLM requirement.

---

## Project Scope

| Area | Decision |
|------|----------|
| Platform | Windows (primary), others future |
| Dimension | 2D (3D is a future milestone) |
| Engine language | C++ (C++20, MSVC / MinGW / Ninja) |
| Gameplay language | C# (Step 12 — not yet implemented) |
| Tooling language | Python (future steps) |
| AI role | External, optional, read/write via Engine API |
| License | Apache 2.0 |

---

## Architecture

Three clean layers with a strict dependency direction:

```
editor
   ↓
runtime
   ↓
core
```

| Layer | Responsibility |
|-------|----------------|
| `core/` | Math, logging, config, UUIDs, JSON, utilities — no rendering or editor dependency |
| `runtime/` | Window, renderer, assets, entities, components, scenes, input, camera, physics, animation, semantic types |
| `editor/` | Editor state, hierarchy panel, inspector panel, viewport panel, asset browser panel |

---

## Build System

- **CMake** 3.25+ with C++20
- **CMake Presets** (`CMakePresets.json`) for reproducible builds
- **vcpkg manifest mode** (`vcpkg.json`) for future C++ dependencies
- **SDL3** fetched via CMake `FetchContent` (release-3.2.8, static)
- **OpenGL** via system `find_package`
- **stb_image** vendored in `third_party/stb/`

### Quick Start (Windows MSVC)

```powershell
# Configure
cmake --preset windows-msvc

# Build (Debug)
cmake --build --preset windows-msvc-debug

# Run all tests
ctest --preset windows-msvc-debug

# Launch editor
.\build\windows-msvc\Debug\KaLaraEditor.exe
```

### Available Presets

| Preset | Generator | Notes |
|--------|-----------|-------|
| `windows-msvc` | Visual Studio 17 2022 | Recommended |
| `windows-ninja` | Ninja | Fast single-config |
| `windows-mingw` | MinGW Makefiles | Standalone GCC |

---

## Development Rules

Read [`AGENTS.md`](./AGENTS.md) before making any changes.

The process is **checkpoint-driven**: the AI agent completes exactly one step, verifies it against acceptance criteria, commits, and stops. The next step begins only after the human explicitly types `CONTINUE`.

---

## Progress

### Completed Steps

| Step | Name | Key Deliverables |
|------|------|-----------------|
| ✅ **00** | Safe Environment | Git, CMake, compiler, build isolation, `.gitignore`, `vcpkg.json`, docs scaffold |
| ✅ **01** | C++ Project Foundation | Logging (`Logger`, macros, levels), `EngineConfig`, `ConfigManager`, `KaLaraCore` static lib |
| ✅ **02** | Window + Main Loop | SDL3 window, OpenGL context, application game loop, update/render split, ESC quit |
| ✅ **03** | Renderer Foundation | `IRenderer` abstract interface, OpenGL backend, VBO/VAO/Shader, `draw_quad` / `draw_triangle` / `draw_line`, viewport abstraction |
| ✅ **04** | Asset Foundation | `UUID` generation, `Image` (stb_image), `Texture2D`, `AssetManager` with ref-counted handles |
| ✅ **05** | Entity + Component Foundation | Stable `EntityID` (uint64_t), `Registry` (type-erased component pools), `TagComponent`, `TransformComponent`, `SpriteRendererComponent`, parent/child hierarchy |
| ✅ **06** | Serialization + Scenes | Custom `JsonValue` (parse + dump), `Scene` class, `SceneSerializer` save/load round-trip |
| ✅ **07** | Input + Camera | `InputManager` (SDL3 keyboard), `KeyCodes`, `Camera2D`, `CameraComponent` |
| ✅ **08** | Physics | `PhysicsWorld2D` (gravity, velocity integration, AABB collision), `Rigidbody2DComponent`, `BoxCollider2DComponent` |
| ✅ **09** | Animation | `SpriteSheet` (grid UV frames), `AnimationClip`, `AnimatorComponent`, `AnimationSystem::update()` |
| ✅ **10** | Editor Foundation | `EditorState` singleton, `EditorHierarchyPanel`, `EditorInspectorPanel`, `EditorViewportPanel`, `EditorAssetBrowserPanel`, `KaLaraEditorLib` |
| ✅ **11** | Semantic System | `SemanticType` enum (Player/Enemy/Wall/…), collision layer bitmasks, `SemanticRegistry` (type inheritance, custom types), `SemanticComponent` (tags, groups, layer/mask) |

### Upcoming Steps

| Step | Name |
|------|------|
| 🔲 **12** | C# Game Layer |
| 🔲 **13** | No-Code Behavior System |
| 🔲 **14** | Template System |
| 🔲 **15** | Project State + Event History |
| 🔲 **16** | Transaction + Undo |
| 🔲 **17** | Windows Export |
| 🔲 **18** | Engine API (public, versioned) |
| 🔲 **19** | AI Bridge (MCP-compatible) |
| 🔲 **20** | Live Context |
| 🔲 **21** | Real-Time Event Channel |
| 🔲 **22** | AI Permissions |
| 🔲 **23** | Multi-Agent Foundation |
| 🔲 **24** | AI Acceptance Tests |

---

## Repository Layout

```
KaLara Engine/
├── core/               # Dependency-light foundations (Math, Log, UUID, JSON, Config)
├── runtime/            # Engine systems (Window, Renderer, Assets, ECS, Scene, Input, Physics, Animation, Semantics)
├── editor/             # Editor application (Hierarchy, Inspector, Viewport, Asset Browser)
├── tests/              # Unit and integration tests (11 test targets via CTest)
├── tools/              # Development tooling (Python, build helpers — future)
├── docs/               # Engineering documentation
├── third_party/        # Reviewed vendored libraries (stb_image)
├── build/              # Generated build output (gitignored)
├── CMakeLists.txt      # Root build definition
├── CMakePresets.json   # Build presets
├── vcpkg.json          # Dependency manifest
└── AGENTS.md           # AI agent development protocol
```

---

## Dependencies

See [`docs/dependencies.md`](./docs/dependencies.md) for the full dependency strategy and version policy.

| Dependency | Version | How |
|-----------|---------|-----|
| SDL3 | release-3.2.8 | CMake FetchContent |
| OpenGL | System | find_package |
| stb_image | latest | Vendored in `third_party/stb/` |
