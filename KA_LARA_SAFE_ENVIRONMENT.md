# KaLara — Safe Development Environment

## Purpose

This document defines the recommended isolated development environment for building KaLara with AI coding agents.

It is separate from the final KaLara runtime architecture.

## Recommended layout

```text
D:\Dev\KaLara\
├── .git\
├── AGENTS.md
├── CMakeLists.txt
├── CMakePresets.json
├── core\
├── runtime\
├── editor\
├── tests\
├── tools\
│   └── python\
├── docs\
├── third_party\
├── build\
├── out\
└── .gitignore
```

Use another user-owned directory when necessary. The important rule is that the project has a clear filesystem boundary.

## Safety rules

The AI development agent should have permission to work inside the KaLara repository.

It should not have unrestricted permission to modify the operating system.

Avoid administrator shells for normal development.

Do not allow development scripts to:

- delete files outside the repository
- modify the Windows registry
- change security settings
- disable antivirus/firewall
- install arbitrary software
- execute unknown downloads
- modify unrelated projects

## Reproducibility

Keep build configuration in the repository.

Use:

- CMake
- a reproducible compiler/toolchain
- a repository-controlled dependency strategy
- project-local Python virtual environments

Avoid global Python package installation.

## Source control

Initialize Git before implementation.

Every checkpoint should leave the repository in a recoverable state.

## Build isolation

Keep generated files under:

```text
build/
out/
dist/
```

Do not commit generated binaries unless the release process explicitly requires them.

## Dependency policy

Record direct dependencies and versions in:

```text
docs/dependencies.md
```

Do not update dependencies merely to resolve unrelated warnings.

## AI-agent safety

During development, the AI agent should use a permission policy equivalent to:

### Allowed automatically

- read repository files
- inspect repository state
- edit repository files
- run approved project-local build/test commands
- create Git commits for completed checkpoints

### Approval required

- install system software
- administrator privileges
- commands that modify files outside the repository
- deleting large directory trees
- changing operating-system configuration
- changing security configuration
- downloading and executing unknown programs

## Recovery

Before every major step:

```text
git status
```

At every checkpoint:

```text
git status
git diff
```

Keep the previous known-good commit available.

## First environment milestone

The environment is ready only when:

1. Git works.
2. CMake works.
3. The selected C++ compiler works.
4. The selected dependency method works.
5. Python virtual environment works.
6. A tiny test program builds.
7. The repository contains no accidental generated files.
8. The agent can operate entirely within the repository boundary.
9. The first Git checkpoint has been committed.
