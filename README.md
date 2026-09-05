# KaLara

**KaLara** is an open-source, human-first, AI-native 2D game engine.

> **Current status:** Step 14 complete — Project Template Generator System implemented.  
> **Next step:** Step 15 — Project State + Event History.

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
| Gameplay language | C# (.NET 8, via P/Invoke bindings — Step 12 ✅) |
| Tooling language | Python (future steps) |
| AI role | External, optional, read/write via Engine API |
| License | Apache 2.0 |

---

## Architecture

Four layers with a strict dependency direction:

```
editor
   ↓
runtime  ←  gameplay (C#, P/Invoke)
   ↓
core
```

| Layer | Language | Responsibility |
|-------|----------|----------------|
| `core/` | C++ | Math, logging, config, UUIDs, JSON, utilities — no rendering or editor dependency |
| `runtime/` | C++ | Window, renderer, assets, ECS, scenes, input, camera, physics, animation, semantics, scripting bridge, behaviors, templates |
| `editor/` | C++ | Editor state, hierarchy, inspector, viewport, asset browser panels |
| `gameplay/csharp/` | C# (.NET 8) | `Entity`, `ScriptableEntity`, math types (`Vector2/3/4`), `InternalCalls` P/Invoke bindings |

---

## Build System

- **CMake** 3.25+ with C++20
- **CMake Presets** (`CMakePresets.json`) for reproducible builds
- **vcpkg manifest mode** (`vcpkg.json`) for future C++ dependencies
- **SDL3** fetched via CMake `FetchContent` (release-3.2.8, static)
- **OpenGL** via system `find_package`
- **stb_image** vendored in `third_party/stb/`
- **C# project** at `gameplay/csharp/KaLara.Engine.csproj` (targets .NET 8)

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

# Build C# gameplay layer (requires .NET 8 SDK)
dotnet build gameplay/csharp/KaLara.Engine.csproj
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
| ✅ **01** | C++ Project Foundation | `Logger` (6 levels, macros), `EngineConfig`, `ConfigManager`, `KaLaraCore` static lib |
| ✅ **02** | Window + Main Loop | SDL3 window, OpenGL context, application game loop, update/render split, ESC quit |
| ✅ **03** | Renderer Foundation | `IRenderer` abstract interface, OpenGL backend, VBO/VAO/Shader, `draw_quad` / `draw_triangle` / `draw_line`, viewport abstraction |
| ✅ **04** | Asset Foundation | `UUID` generation (`uint64_t`), `Image` (stb_image), `Texture2D`, `AssetManager` with ref-counted handles |
| ✅ **05** | Entity + Component Foundation | Stable `EntityID`, `Registry` (type-erased component pools via `std::any`), `TagComponent`, `TransformComponent`, `SpriteRendererComponent`, parent/child `Hierarchy` |
| ✅ **06** | Serialization + Scenes | Custom `JsonValue` (parse + dump), `Scene` class, `SceneSerializer` save/load round-trip |
| ✅ **07** | Input + Camera | `InputManager` (SDL3 keyboard), `KeyCodes`, `Camera2D`, `CameraComponent` |
| ✅ **08** | Physics | `PhysicsWorld2D` (gravity, semi-implicit Euler, AABB collision), `Rigidbody2DComponent`, `BoxCollider2DComponent` |
| ✅ **09** | Animation | `SpriteSheet` (grid UV frames), `AnimationClip`, `AnimatorComponent`, `AnimationSystem::update()` |
| ✅ **10** | Editor Foundation | `EditorState` singleton (selection, gizmo modes, save/load), `EditorHierarchyPanel`, `EditorInspectorPanel`, `EditorViewportPanel`, `EditorAssetBrowserPanel`, `KaLaraEditorLib` |
| ✅ **11** | Semantic System | `SemanticType` enum (Player/Enemy/Wall/Collectible/Trigger/Projectile/Custom), collision layer bitmasks, `SemanticRegistry` (type inheritance, custom types), `SemanticComponent` (tags, groups, layer/mask) |
| ✅ **12** | C# Game Layer | `KaLara.Engine` (.NET 8 C# project), `Entity` + `ScriptableEntity` base classes, `Vector2/3/4` math types, `InternalCalls` P/Invoke layer, C++ `ScriptEngine` with `extern "C"` bindings for Log / Entity / Transform / Input |
| ✅ **13** | No-Code Behavior System | `MovementBehaviorComponent` (speed, jump, input controls), `HealthBehaviorComponent` (take_damage, heal), `DamageBehaviorComponent`, `ShootingBehaviorComponent` (fire rate, cooldown), `CollectibleBehaviorComponent` (score, heal), `CheckpointBehaviorComponent`, `DoorBehaviorComponent`, `TriggerBehaviorComponent`, `BehaviorSystem::update()` |
| ✅ **14** | Template System | `TemplateManager` with 4 project templates: **Blank**, **Platformer** (player + physics ground), **Top-Down RPG** (player + collectibles + triggers), **Top-Down Shooter** (player + shooting + enemy). Each template generates a full project folder (`assets/`, `scenes/`, `scripts/`), `project.json` manifest, serialized default scene, and a starter `PlayerController.cs` |

### Upcoming Steps

| Step | Name |
|------|------|
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
├── core/                    # Dependency-light foundations (Math, Log, UUID, JSON, Config)
├── runtime/                 # Engine systems (Window, Renderer, Assets, ECS, Scene,
│                            #   Input, Camera, Physics, Animation, Semantics,
│                            #   ScriptEngine, BehaviorSystem, TemplateManager)
├── editor/                  # Editor application (Hierarchy, Inspector, Viewport, Asset Browser)
├── gameplay/
│   └── csharp/              # C# gameplay layer (.NET 8)
│       ├── Core/            #   Entity, ScriptableEntity, Vector2/3/4, Log
│       ├── Native/          #   InternalCalls (P/Invoke bindings to C++ engine)
│       └── KaLara.Engine.csproj
├── tests/                   # Unit and integration tests (14 test targets via CTest)
├── tools/                   # Development tooling (Python — future)
├── docs/                    # Engineering documentation
├── third_party/             # Reviewed vendored libraries (stb_image)
├── build/                   # Generated build output (gitignored)
├── CMakeLists.txt           # Root build definition
├── CMakePresets.json        # Build presets
├── vcpkg.json               # Dependency manifest
└── AGENTS.md                # AI agent development protocol
```

---

## Dependencies

See [`docs/dependencies.md`](./docs/dependencies.md) for the full dependency strategy and version policy.

| Dependency | Version | How | Used For |
|-----------|---------|-----|---------|
| SDL3 | release-3.2.8 | CMake FetchContent | Window, OpenGL context, events, input |
| OpenGL | System | find_package | Rendering backend |
| stb_image | latest | Vendored (`third_party/stb/`) | Image loading (PNG, JPG, BMP…) |
| .NET 8 SDK | 8.x | System prerequisite | C# gameplay layer compilation |

---

## V1 Acceptance Goals

KaLara's success is measured by three workflows, not feature count:

### Workflow A — Beginner (No Code)
```
Open KaLara → Choose template → Answer questions
→ Playable project → Place objects → Configure behaviors → Run
```
> Templates and no-code behaviors are now implemented (Steps 13 & 14). ✅ Foundation ready.

### Workflow B — AI-Assisted
```
Open blank project → Connect external AI → Describe game
→ AI creates structure → User supplies assets → Human + AI iterate → Playable game
```
> Requires AI Bridge (Step 19) and Engine API (Step 18). 🔲 Pending.

### Workflow C — Selection-Aware AI
```
Select objects → Tell AI "These are walls"
→ AI applies semantics + collision + group → One transaction → Undo works
```
> Semantic system done (Step 11). Transaction/Undo (Step 16) and AI Bridge (Step 19) pending. 🔲 Partially ready.
