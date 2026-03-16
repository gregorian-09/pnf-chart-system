# Getting Started Overview

Use this path when onboarding to the repository.

1. Install build and runtime dependencies.
2. Build the C++ core.
3. Run the full test matrix.
4. Run one end-to-end sample for your target binding.

## Onboarding Flow

```mermaid
sequenceDiagram
  participant Dev as Developer
  participant Repo as Repository
  participant Core as C++ Core
  participant Bind as Bindings
  participant Tests as Test Matrix

  Dev->>Repo: clone repository
  Dev->>Repo: install toolchains
  Dev->>Core: cmake configure/build
  Core-->>Dev: native libraries
  Dev->>Bind: build language bindings
  Dev->>Tests: run C++, Python, Java, Rust, C# tests
  Tests-->>Dev: pass/fail report
```

## Entry Points

- [Install Prerequisites](install.md)
- [Build and Test](build-and-test.md)
- [First Chart Walkthrough](first-chart.md)
