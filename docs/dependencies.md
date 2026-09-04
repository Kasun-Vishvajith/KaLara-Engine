# KaLara Dependency Strategy

## Step 00 policy

Step 00 intentionally installs **no third-party runtime/library dependency**. The first checkpoint proves that the local toolchain and repository work before SDL3, OpenGL-facing code, physics, UI, image loading, or C# infrastructure are introduced.

This is deliberate: a clean toolchain checkpoint makes later dependency failures easier to isolate.

## Package manager

KaLara will use **vcpkg manifest mode** as the default C/C++ dependency strategy.

Reasons:

- good Windows/CMake integration
- dependency manifests live in the repository
- supports version control/baselines
- works with Visual Studio and command-line CMake
- avoids manually copying libraries into the source tree

The project will keep dependency configuration in `vcpkg.json` and `vcpkg-configuration.json`.

## Dependency rules

1. Add a dependency only when a milestone actually needs it.
2. Prefer mature upstream libraries over custom implementations for non-core infrastructure.
3. Pin the registry baseline before the first dependency is committed.
4. Record each direct dependency and its reason in this document.
5. Do not update all dependencies as a batch.
6. Do not vendor third-party source into `third_party/` unless there is a specific reviewed reason.
7. License compatibility must be checked before adding a dependency.
8. Build configuration must work from a clean checkout.

## Planned dependency order

### Step 01/02 — Windowing and platform

Add:

- SDL3 (version 3.2.8 via CMake FetchContent / release target)

Use SDL3 for window creation, OpenGL context creation, and input abstraction. SDL3 officially supports CMake and Visual Studio on Windows. The project links the `SDL3::SDL3` CMake target.

### Rendering

OpenGL is a graphics API, not a package that needs to be pulled in as a normal third-party library on Windows. SDL3 creates/manages the OpenGL context. KaLara will wrap actual GL calls behind its renderer abstraction.

A dedicated OpenGL loader dependency will be considered when the renderer actually needs one. Do not add one in Step 00.

### Image loading

Candidate:

- stb_image

Add only when the sprite/texture milestone starts.

### Physics

Candidate:

- Box2D

Add only when the physics milestone starts.

### Editor UI

Candidate:

- Dear ImGui

Add only when the editor milestone starts.

### JSON

A JSON library will be selected when serialization begins. Prefer a small, well-maintained C++ library with a straightforward data model. The exact library is intentionally not locked in Step 00.

### C# runtime

Do not add a C# runtime in Step 00. The exact embedding/hosting approach must be decided and tested when the C# milestone begins.

## Version policy

The repository will use a vcpkg registry baseline plus explicit dependency versions/overrides where needed. The baseline must be derived from a real vcpkg Git commit, not an invented identifier.

The current upstream vcpkg registry contains SDL3 and supports manifest-mode dependency/version control. Before SDL3 is added, update this file and the manifest together with the exact verified baseline and selected SDL3 version.

## Reproducibility

A dependency-enabled checkpoint is complete only when:

```text
clean checkout
    ↓
vcpkg manifest resolution
    ↓
CMake configure
    ↓
build
    ↓
test
```

works without undocumented machine-specific setup.

## System dependencies

Normal Windows developer prerequisites such as a C++ compiler, Git, CMake, and an optional Ninja installation are considered **toolchain prerequisites**, not project libraries.

The agent must never silently install system dependencies.

## Future lock file / binary caching

After the first dependency-enabled milestone, evaluate vcpkg binary caching for CI and local rebuild speed. This is not required for Step 00.
