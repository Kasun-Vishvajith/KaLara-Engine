# KaLara

**KaLara** is an open-source, human-first, AI-native 2D game engine.

This repository is currently at **Step 00 — Safe Environment / Repository Foundation**.

## Development rule

Read [`AGENTS.md`](./AGENTS.md) before making changes.

The development process is checkpoint-driven. The AI coding agent completes one step, verifies it, documents it, and stops. The next step begins only after the human explicitly types `CONTINUE`.

## Current scope

- Windows first
- 2D first
- C++ engine/runtime
- C# gameplay layer later
- Python tooling later
- AI is external and optional
- Apache License 2.0

## Build system

- CMake
- CMake Presets
- vcpkg manifest mode for project dependencies

## Step 00

Step 00 deliberately contains no third-party runtime dependencies. Its purpose is to prove that the project can be built and tested in a controlled environment before dependencies are introduced.
