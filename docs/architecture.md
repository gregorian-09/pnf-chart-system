# Architecture Overview

This document gives a top-to-bottom view of the system. For deeper functional behavior, continue into the `concepts/` section.

## Layered Architecture

```mermaid
flowchart TD
  App[Applications] --> Lang[Language Bindings]
  Lang --> CABI[C ABI Boundary]
  CABI --> Core[C++ Core Engine]
  Core --> Analytics[Indicators and Pattern Engines]
  Core --> Trend[Trendline Manager]
  Core --> Export[Visualization and Data Export]
```

## Runtime Call Topology

```mermaid
sequenceDiagram
  participant User
  participant Binding
  participant C as C ABI
  participant CPP as C++ Core

  User->>Binding: add data / calculate indicators
  Binding->>C: marshal and call
  C->>CPP: execute core logic
  CPP-->>C: result data
  C-->>Binding: transformed return values
  Binding-->>User: language-native objects
```

## Internal Components

- Chart construction and mutation: `Chart`
- Column and box model: `Column`, `Box`
- Trendline maintenance: `TrendLineManager`
- Indicator stack: `Indicators` and helper analyzers
- Visualization/export: `Visualization` and export structs

## Deep Dives

- [Chart Model](concepts/chart-model.md)
- [Chart Construction Rules](concepts/chart-construction.md)
- [Indicators and Signals](concepts/indicators-and-signals.md)
- [Patterns and Objectives](concepts/patterns-and-objectives.md)
- [Trendlines and Bias](concepts/trendlines-and-bias.md)
