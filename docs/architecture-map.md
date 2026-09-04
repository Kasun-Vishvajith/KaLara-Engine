# KaLara Repository Architecture Map

```text
KaLara/
├── core/       # dependency-light foundations
├── engine/     # runtime systems
├── editor/     # human editor/application
├── tests/      # automated verification
├── tools/      # development tooling (future Python)
├── docs/       # engineering documentation
├── third_party/ # reserved; do not vendor casually
├── build/      # generated; ignored
├── out/        # generated; ignored
└── dist/       # generated; ignored
```

Dependency direction:

```text
editor → engine → core
```

The runtime must never depend on the editor.
