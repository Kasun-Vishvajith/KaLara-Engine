# KaLara — AGENTS.md
## Controlled AI Development Protocol

**Project:** KaLara  
**Purpose:** Development-time instructions for AI coding agents  
**Status:** Authoritative development controller  
**Scope:** Building the KaLara engine only  
**Important:** This document is NOT the runtime AI-agent specification for users of finished KaLara projects.

---

# 0. READ THIS FIRST

You are an AI coding agent working on the **KaLara game engine**.

Your job is to build KaLara safely, incrementally, and predictably.

You MUST:

1. Work on exactly one development step at a time.
2. Read the current step before editing code.
3. Never skip a checkpoint.
4. Never silently expand scope.
5. Never rewrite completed architecture without a documented reason.
6. Keep the project buildable at every checkpoint.
7. Prefer small, reversible changes.
8. Use the repository as the source of truth.
9. Stop at every checkpoint.
10. Ask the human to review the result.
11. Do NOT continue until the human explicitly types:
   `CONTINUE`
12. If the human types anything else, treat it as review/change instructions and do not advance automatically.

The agent must never say that a step is complete when the acceptance criteria have not been demonstrated.

---

# 1. PRODUCT IDENTITY

KaLara is an open-source, human-first, AI-native game engine.

Core goal:

> Make it possible for beginners to create real games without programming while providing external AI coding agents with a structured, machine-readable way to understand and modify the same project.

KaLara must work completely without AI.

AI is an optional external development assistant.

KaLara itself does NOT contain an AI model.

Initial target:

- Windows
- 2D
- Open source
- Apache License 2.0

Future:

- 3D
- Multiplayer
- More platforms

These are NOT V1 implementation priorities.

---

# 2. AUTHORITATIVE ARCHITECTURE

Use this architecture unless the human explicitly changes it.

## Core languages

### C++
Engine/runtime.

Responsible for:

- core systems
- memory/runtime infrastructure
- rendering
- physics integration
- input abstraction
- scene/runtime
- entity/component infrastructure
- asset runtime
- serialization
- engine API

### C#
Game development layer.

Responsible for:

- gameplay
- rules
- custom behaviors
- state machines
- game-specific systems
- calculations
- advanced gameplay logic

### Python
Tooling layer.

Responsible for:

- asset processing
- project tooling
- import/export tools
- automation
- build helpers
- development utilities
- AI/development utilities

Python is NOT the primary runtime gameplay language.

---

# 3. LAYER SEPARATION

The repository MUST maintain three clean conceptual layers from the beginning:

```text
core/
    Math
    Containers
    IDs
    Logging
    Serialization primitives
    Utilities

runtime/
    Entities
    Components
    Systems
    Scene
    Rendering
    Physics
    Input
    Audio
    Assets
    Runtime API

editor/
    Viewport
    Hierarchy
    Inspector
    Asset Browser
    Tools
    Editor state
    UI
```

Dependency direction:

```text
editor
   ↓
runtime
   ↓
core
```

`core` must not depend on rendering or editor systems.

`runtime` must never depend on editor code.

The exported game must never require the editor.

---

# 4. NON-NEGOTIABLE ARCHITECTURAL RULES

## Rule 1 — Stable Entity IDs

Every entity gets a persistent unique ID.

Do NOT use:

- raw pointer identity
- vector index as identity
- temporary names as identity

Entity names may change.

Entity IDs must remain stable.

Example:

```text
entity_8f32a1
```

---

## Rule 2 — One Mutation Choke Point

All meaningful project/entity/component mutations must pass through the engine's controlled API.

Do NOT scatter direct state writes throughout the codebase.

Preferred pattern:

```text
UI
 ↓
Engine API
 ↓
State mutation
 ↓
Future event/transaction hook
```

This rule must exist BEFORE AI integration.

---

## Rule 3 — Serialize Early

Important project state must be serializable from the beginning.

The first project representation may use JSON.

Serialization must support:

```text
state
 ↓
save
 ↓
load
 ↓
same state
```

No feature should depend on undocumented hidden state where practical.

---

## Rule 4 — Renderer Abstraction

Do not scatter raw OpenGL calls through the entire engine.

Use a renderer abstraction.

Initial backend:

```text
OpenGL
```

Future backends may include:

```text
Vulkan
DirectX
```

Do not implement future backends during V1 unless explicitly instructed.

---

## Rule 5 — Human and AI Eventually Use the Same API

The final architecture is:

```text
Human Editor
      \
       \
        → Engine API → Project State
       /
AI Agent
```

Do NOT create a separate secret mutation path for AI.

---

## Rule 6 — AI Is Not a Runtime Dependency

The shipped game must run without:

- AI connection
- AI service
- cloud service
- LLM
- editor

---

## Rule 7 — No Unnecessary Complexity

Do not introduce:

- microservices
- cloud infrastructure
- vector databases
- distributed systems
- unnecessary networking
- multiple language runtimes

unless explicitly required by the current step.

---

# 5. DEVELOPMENT SAFETY ENVIRONMENT

The AI agent MUST work inside an isolated project workspace.

## Recommended host layout

Use a dedicated development directory such as:

```text
D:\Dev\KaLara\
```

or another user-owned development directory.

Do NOT develop inside:

```text
C:\
C:\Windows\
C:\Program Files\
C:\Program Files (x86)\
Windows system directories
```

except where a normal installer/toolchain requires system installation.

---

## Repository boundary

The Git repository is the primary writable boundary.

The agent MUST NOT modify files outside the repository unless the current step explicitly requires it.

The agent MUST NOT:

- delete unrelated files
- modify registry settings
- disable antivirus/firewall
- change OS security settings
- modify boot settings
- modify unrelated environment variables
- install random software
- execute downloaded unknown binaries
- run destructive shell commands

---

## Privilege rule

Never request or use administrator privileges unless the human explicitly approves it for a specific installation.

Prefer:

- user-level installs
- project-local dependencies
- repository-local tools
- virtual environments

---

## Python environment

Use a project-local virtual environment:

```text
tools/python/.venv/
```

Never install project Python dependencies globally.

Preferred:

```text
python -m venv tools/python/.venv
```

then activate/use that environment.

---

## Dependency rule

Prefer pinned, reproducible dependencies.

Do not casually update all dependencies.

A dependency change must be treated as an architectural change and recorded in the checkpoint notes.

---

## C++ dependencies

Prefer a repository-controlled package/dependency strategy.

The selected package manager and exact dependency versions must be recorded in:

```text
docs/dependencies.md
```

Do not download arbitrary third-party source code during an unrelated task.

---

## Build directory

Build outputs must be isolated:

```text
build/
out/
dist/
```

Generated files must not pollute source directories.

---

## Secrets

Never store:

- API keys
- passwords
- access tokens
- private credentials
- personal secrets

inside the repository.

Use:

```text
.env
```

only when necessary and ensure it is ignored by Git.

There is no AI API key requirement for the engine itself.

---

# 6. GIT SAFETY

Git must be initialized before substantial implementation begins.

The agent should create small logical commits.

Recommended format:

```text
step-00: establish safe development environment
step-01: establish core project
step-02: create window and loop
...
```

Never rewrite or squash history without human approval.

Before starting a step:

```text
git status
```

At checkpoint:

```text
git status
git diff
```

The agent must report:

- changed files
- added files
- deleted files
- build result
- tests
- known issues

---

# 7. WORKFLOW FOR EVERY STEP

Every development step follows exactly this process.

```text
READ
 ↓
UNDERSTAND
 ↓
INSPECT CURRENT STATE
 ↓
PLAN
 ↓
IMPLEMENT
 ↓
BUILD
 ↓
TEST
 ↓
CHECK ACCEPTANCE CRITERIA
 ↓
DOCUMENT
 ↓
COMMIT
 ↓
CHECKPOINT
 ↓
STOP
```

Do not automatically start the next step.

---

# 8. BEFORE MODIFYING ANY CODE

The agent MUST first inspect:

1. `AGENTS.md`
2. current roadmap/checkpoint
3. repository status
4. current project structure
5. relevant source files
6. relevant tests
7. current build result if needed

The agent must understand the existing implementation before editing it.

Do not replace existing files blindly.

---

# 9. STEP SCOPE CONTROL

Each step has:

- objective
- inputs
- allowed changes
- implementation tasks
- tests
- acceptance criteria
- checkpoint

The agent MUST NOT perform future-step work "because it is convenient."

Example:

If the current step is:

```text
Create SDL3 window
```

do not additionally implement:

- entity systems
- physics
- AI bridge
- editor
- templates

unless needed to satisfy the current acceptance criteria.

---

# 10. CHECKPOINT PROTOCOL

At the end of every step, the agent MUST stop.

The checkpoint response must use this structure:

```text
CHECKPOINT: STEP XX

Status:
PASS / PASS WITH NOTES / BLOCKED

Implemented:
- ...

Files changed:
- ...

Build:
PASS / FAIL

Tests:
- ...

Acceptance criteria:
- PASS ...
- PASS ...
- ...

Known issues:
- ...

Commit:
<commit hash>

Review required.

Type CONTINUE to start the next step.

Or describe any changes you want before continuing.
```

The agent MUST NOT continue after displaying this message.

---

# 11. CONTINUE RULE

Only the exact instruction:

```text
CONTINUE
```

moves the project to the next step.

Case-insensitive matching is acceptable.

Examples that do NOT automatically authorize continuation:

```text
looks good
nice
okay
yes
fine
go ahead
```

Unless the human explicitly uses:

```text
CONTINUE
```

the agent must remain at the current checkpoint.

If the human provides change instructions, implement those changes first and create a new checkpoint.

---

# 12. REVIEW CHANGE PROTOCOL

If the human says:

> Change the renderer structure.

The agent must:

1. remain on the current checkpoint
2. interpret the requested change
3. modify the project
4. rebuild/test
5. produce another checkpoint
6. wait for `CONTINUE`

Do not silently advance.

---

# 13. BLOCKED PROTOCOL

If a step cannot be completed safely:

```text
CHECKPOINT: STEP XX

Status:
BLOCKED

Reason:
...

What was attempted:
...

Evidence:
...

Recommended choices:
A. ...
B. ...

No next step has been started.
```

Do not hide failures.

Do not fake a PASS.

---

# 14. ERROR POLICY

If a build fails:

1. diagnose the failure
2. fix only what belongs to the current step
3. rebuild
4. test again

If fixing it would require future architecture work:

STOP and report it.

Do not create a large speculative rewrite.

---

# 15. CODE QUALITY RULES

Prefer:

- readable code
- small modules
- explicit ownership
- clear interfaces
- deterministic behavior
- testable logic
- descriptive names
- comments only where they add real value

Avoid:

- premature abstraction
- unnecessary template metaprogramming
- giant classes
- global mutable state
- hidden side effects
- magic constants
- untracked generated files

---

# 16. TESTING RULE

Every completed step needs evidence.

Depending on the step, evidence may be:

- unit test
- integration test
- executable run
- visual/manual test
- serialization round-trip test
- performance measurement
- API test

For visual/editor features, a manual test is acceptable when automated testing is not practical.

Record what was tested.

---

# 17. PERFORMANCE RULE

Do not optimize blindly.

First make the architecture correct and measurable.

Use:

- simple profiling
- frame-time measurements
- memory measurements
- targeted benchmarks

Do not sacrifice architecture for unmeasured theoretical performance.

---

# 18. ARCHITECTURE CHANGE RULE

If the agent discovers that the current architecture must change:

DO NOT silently change it.

Use:

```text
ARCHITECTURE CHANGE REQUEST

Current decision:
...

Problem:
...

Evidence:
...

Proposed change:
...

Affected systems:
...

Migration risk:
...

Human approval required.
```

Wait for the human's decision.

---

# 19. SOURCE-OF-TRUTH DOCUMENTS

Use this priority order:

1. `AGENTS.md`
2. human-approved architecture decisions
3. current implementation
4. roadmap
5. older draft/spec documents

If documents conflict, do not silently choose.

The current approved architecture takes priority.

---

# 20. IMPORTANT CONFLICT FROM EARLIER ROADMAP

An older roadmap may suggest temporarily using Lua or AngelScript before C#.

That is NOT the final KaLara direction.

The approved direction is:

```text
C++ Engine
      +
C# Game Layer
      +
Python Tooling
```

Do not introduce Lua or AngelScript merely as a temporary shortcut unless the human explicitly approves it as a temporary development experiment.

The goal is to validate the actual C# architecture, not build a throwaway scripting architecture first.

---

# 21. DEVELOPMENT PHASES

The development process is split into controlled phases.

## STEP 00 — SAFE ENVIRONMENT

Prove:

- Git
- compiler/toolchain
- CMake
- dependency management
- Python environment
- local build
- project directory boundary

No engine feature work yet.

---

## STEP 01 — C++ PROJECT FOUNDATION

Create:

```text
core
runtime
editor
tests
tools
docs
```

Establish:

- CMake
- build presets
- logging
- configuration
- basic application target

---

## STEP 02 — WINDOW + MAIN LOOP

Implement:

- SDL3
- OpenGL context
- application loop
- update/render split
- clean shutdown
- ESC quit

First visible proof:

```text
KaLara window
+
one rectangle
```

---

## STEP 03 — RENDERER FOUNDATION

Implement:

- renderer interface
- OpenGL backend
- basic geometry
- viewport abstraction
- GPU resource ownership

---

## STEP 04 — ASSET FOUNDATION

Implement:

- image loading
- texture loading
- resource handles
- cache
- asset metadata foundation

---

## STEP 05 — ENTITY + COMPONENT FOUNDATION

Implement:

- stable EntityID
- entity registry
- component storage
- Transform
- parent/child hierarchy

---

## STEP 06 — SERIALIZATION + SCENES

Implement:

- scene
- entities
- components
- JSON serialization
- save/load
- round-trip testing

---

## STEP 07 — INPUT + CAMERA

Implement:

- input abstraction
- keyboard
- camera
- basic player movement

---

## STEP 08 — PHYSICS

Integrate a mature 2D physics backend.

Implement the V1-required physics foundations.

---

## STEP 09 — ANIMATION

Implement:

- sprite sheets
- frames
- named animations
- animation states
- blend-tree foundation

---

## STEP 10 — EDITOR FOUNDATION

Implement:

- viewport
- hierarchy
- inspector
- asset browser
- selection
- move/rotate/scale
- save/load UI

---

## STEP 11 — SEMANTIC SYSTEM

Implement:

- semantic types
- inheritance
- tags
- groups
- collision categories/layers
- built-in semantic vocabulary
- custom semantic types

---

## STEP 12 — C# GAME LAYER

Implement:

- C# project
- compiler integration
- engine bindings
- entity access
- component access
- gameplay API
- diagnostics

This must use the same underlying engine API.

---

## STEP 13 — NO-CODE BEHAVIOR SYSTEM

Implement beginner-facing configuration for common gameplay behaviors.

Examples:

- movement
- health
- damage
- enemy targeting
- shooting
- collectibles
- checkpoints
- doors
- triggers

C# remains available for advanced users.

---

## STEP 14 — TEMPLATE SYSTEM

Implement:

- Blank Project
- Platformer
- Top-Down RPG
- Top-Down Shooter

Templates must create usable playable starting projects with placeholders.

---

## STEP 15 — PROJECT STATE + EVENT HISTORY

Implement:

- structured project state
- event model
- actor type
- transaction ID
- history
- rollback foundation
- project summaries
- tasks
- decisions
- errors

---

## STEP 16 — TRANSACTION + UNDO

Implement:

- logical transactions
- AI-style transaction grouping
- human grouped operations where appropriate
- undo
- redo
- before/after state tracking

---

## STEP 17 — WINDOWS EXPORT

Implement:

- standalone packaging
- runtime-only build
- asset packaging
- configuration
- executable output

The exported game must not require the editor.

---

## STEP 18 — ENGINE API

Finalize the public semantic Engine API.

Initial categories:

```text
project
scene
entity
component
group
asset
script
history
context
game
```

The API must be versioned and documented.

---

## STEP 19 — AI BRIDGE

ONLY NOW begin the AI development interface.

Implement:

- agent protocol
- MCP compatibility
- local bridge
- authentication/authorization model
- tool discovery
- structured responses
- safe mutation path

No embedded model.

---

## STEP 20 — LIVE CONTEXT

Implement:

- current scene
- current selection
- inspector context
- recent changes
- project state
- relevant assets
- errors
- game state
- project summary

---

## STEP 21 — REAL-TIME EVENT CHANNEL

Implement lightweight local real-time synchronization.

Rules:

- meaningful changes are emitted
- continuous drag events are coalesced
- committed state is authoritative
- AI can retrieve changes since its last known version

---

## STEP 22 — AI PERMISSIONS

Implement:

- per-agent permissions
- per-session/task permissions
- read permissions
- write permissions
- dangerous-operation approval
- audit records

---

## STEP 23 — MULTI-AGENT FOUNDATION

Allow multiple external agents to connect safely.

Do not prioritize advanced collaboration until single-agent reliability is excellent.

---

## STEP 24 — AI ACCEPTANCE TESTS

Required demonstrations:

1. AI reads project.
2. AI reads current selection.
3. AI understands semantic entities.
4. AI modifies entities.
5. AI modifies components.
6. Human modification appears to AI.
7. AI modification appears to human.
8. AI transaction can be undone.
9. AI can create a small playable prototype.
10. Multiple agents can eventually connect without corrupting state.

---

# 22. V1 "DO NOT BUILD" LIST

Until the human explicitly opens a later phase, do not prioritize:

- full 3D
- multiplayer
- embedded AI model
- cloud platform
- console export
- mobile export
- AAA renderer
- dozens of programming languages
- arbitrary shell execution through AI
- automatic generative asset creation
- huge visual scripting system
- unnecessary distributed architecture

---

# 23. V1 ACCEPTANCE TEST

The project is not considered successful because the engine has many features.

It succeeds when these workflows work.

## Workflow A — Beginner

```text
Open KaLara
 ↓
Choose Platformer
 ↓
Answer simple questions
 ↓
Playable project created
 ↓
Place objects
 ↓
Configure behaviors
 ↓
Run
 ↓
No C# required
```

## Workflow B — AI-assisted

```text
Open blank project
 ↓
Connect external AI
 ↓
Describe game
 ↓
AI creates project structure
 ↓
Placeholders appear
 ↓
User supplies assets
 ↓
Human + AI iterate
 ↓
Playable game
```

## Workflow C — Selection-aware

```text
Select 3 rectangles
 ↓
Tell AI:
"These are the walls."
 ↓
AI identifies selection
 ↓
Wall semantics
 ↓
Collision
 ↓
Group
 ↓
One transaction
 ↓
Undo works
```

---

# 24. FINAL RULE

The agent's purpose is not to finish KaLara as fast as possible.

The agent's purpose is to build KaLara:

- safely
- correctly
- transparently
- incrementally
- reversibly
- with human review at every major checkpoint

When uncertain:

STOP.

Explain the uncertainty.

Ask for review.

Do not guess on architectural decisions.

---

# 25. FIRST COMMAND

When the repository is ready, the first task is:

```text
STEP 00 — SAFE ENVIRONMENT
```

Do not start engine implementation before Step 00 passes.

At the end of Step 00:

STOP.

Ask for:

```text
CONTINUE
```

Only then begin Step 01.
