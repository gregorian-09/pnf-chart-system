# CI/CD Design

## Pipeline Stages

```mermaid
flowchart TD
  A[Checkout] --> B[Build Core]
  B --> C[Run Version Consistency Check]
  C --> D[Run API Docs Coverage Check]
  D --> E[Run C++ Tests]
  E --> F[Run Python Tests]
  F --> G[Run Java Tests]
  G --> H[Run Rust Tests]
  H --> I[Run C# Tests]
  I --> J[Package]
  J --> K[Publish on tag]
```

## Principles

- Fail fast on compile or core test errors.
- Fail fast when docs/version governance checks fail.
- Do not publish unless full matrix passes.
- Publish only from tagged commits.
- Use per-registry scoped secrets.

## Baseline Verification Commands

```bash
python3 tools/check_versions.py
python3 tools/generate_api_symbol_index.py
python3 tools/check_docs_coverage.py
```

## Required Secrets (when publishing)

- `PYPI_API_TOKEN`
- Maven Central credentials/token pair
- `CARGO_REGISTRY_TOKEN`
- `NUGET_API_KEY`
- GPG material for Maven Central signing
