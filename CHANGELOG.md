# Changelog

All notable changes to this project are documented in this file.

This format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project follows [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.1.2] - 2026-03-17

### Fixed
- Python source distribution now bundles the core C++/CMake sources required for `pip` build-from-source installs.
- Python build path resolution now works in both repository and sdist layouts.
- Python CMake build disables Java/Rust subprojects when building wheels from the Python package.
- NuGet package now includes native runtime assets under `runtimes/*/native`, enabling runtime loading without manual native library copying.

## [0.1.1] - 2026-03-17

### Added
- Generated API symbol coverage index at `docs/reference/api-symbol-index.md`.
- Version governance tooling via `tools/check_versions.py` and `tools/generate_api_symbol_index.py`.
- Canonical root `VERSION` file used by Python packaging metadata.

### Changed
- Expanded public API documentation across C++, C ABI, Python, Java, Rust, and C# docs.
- Strengthened release/versioning documentation and synchronized version policy.

## [0.1.0] - 2026-03-17

### Added
- Core Point & Figure C++ engine (`Chart`, trendline, indicators, visualization/export).
- Stable C ABI for cross-language integration.
- Python, Java, Rust, and C# bindings.
- Localhost dashboard server implementations across bindings.
- Build and test support for Linux toolchains and binding-specific workflows.

### Changed
- Documentation set expanded with architecture, concepts, operations, and API pages.

### Fixed
- Documentation comment quality improvements for Java and Rust bindings.

[Unreleased]: https://github.com/gregorian-09/pnf-chart-system/compare/v0.1.2...HEAD
[0.1.2]: https://github.com/gregorian-09/pnf-chart-system/compare/v0.1.1...v0.1.2
[0.1.1]: https://github.com/gregorian-09/pnf-chart-system/compare/v0.1.0...v0.1.1
[0.1.0]: https://github.com/gregorian-09/pnf-chart-system/releases/tag/v0.1.0
