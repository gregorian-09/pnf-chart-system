# CI/CD Design

## Pipeline Stages

```mermaid
flowchart TD
  A[Checkout] --> B[Build Core]
  B --> C[Run C++ Tests]
  C --> D[Run Python Tests]
  D --> E[Run Java Tests]
  E --> F[Run Rust Tests]
  F --> G[Run C# Tests]
  G --> H[Package]
  H --> I[Publish on tag]
```

## Principles

- Fail fast on compile or core test errors.
- Do not publish unless full matrix passes.
- Publish only from tagged commits.
- Use per-registry scoped secrets.

## Required Secrets (when publishing)

- `PYPI_API_TOKEN`
- Maven Central credentials/token pair
- `CARGO_REGISTRY_TOKEN`
- `NUGET_API_KEY`
- GPG material for Maven Central signing
