# Step 00 — Safe Environment Checklist

## Goal

Prove that the KaLara repository can be configured, compiled, tested, and run without touching unrelated parts of the host system.

## Recommended Windows tools

- Git
- CMake 3.25+
- Visual Studio 2022 / Build Tools with Desktop development with C++
- Windows 10/11 SDK through the Visual Studio C++ workload
- Ninja is optional
- Python 3.x for future tooling; Step 00 only requires a local virtual environment to be possible
- vcpkg for future project dependencies

## Repository boundary

The agent should operate inside this repository only:

```text
<development-root>/KaLara/
```

Generated files belong under the repository's ignored build/output directories.

## Safe operating rules

- No administrator shell for normal development.
- No registry edits.
- No security-setting changes.
- No antivirus/firewall changes.
- No deletion outside the repository.
- No unknown downloaded executable runs.
- No global Python package installation for KaLara tooling.
- No project dependency installation without the current step requiring it.

## Step 00 commands

### Verify Git

```powershell
git --version
```

### Verify CMake

```powershell
cmake --version
```

### Verify C++ compiler

From a Visual Studio Developer PowerShell/Developer Command Prompt:

```powershell
cl
```

### Configure

```powershell
cmake --preset windows-msvc
```

### Build

```powershell
cmake --build --preset windows-msvc-debug
```

### Test

```powershell
ctest --preset windows-msvc-debug
```

### Run

```powershell
.\build\windows-msvc\Debug\KaLaraEditor.exe
```

## Expected output

```text
KaLara development foundation 0.0.0
Runtime foundation status: 1
```

## Checkpoint

Do not start Step 01 until the agent has shown:

- clean/understood Git status
- successful CMake configure
- successful build
- successful smoke test
- successful executable run
- list of files changed
- commit hash

Then stop and request:

```text
CONTINUE
```
