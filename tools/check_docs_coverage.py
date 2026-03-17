#!/usr/bin/env python3
"""Validate API documentation symbol coverage for all public surfaces."""

from __future__ import annotations

import re
import sys
from pathlib import Path

from generate_api_symbol_index import (
    ROOT,
    extract_c_symbols,
    extract_cpp_symbols,
    extract_csharp_symbols,
    extract_java_symbols,
    extract_python_symbols,
    extract_rust_symbols,
)

INDEX_PATH = ROOT / "docs" / "reference" / "api-symbol-index.md"


def flatten(grouped: dict[str, list[str]]) -> set[str]:
    merged: set[str] = set()
    for values in grouped.values():
        merged.update(values)
    return merged


def parse_index_symbols() -> dict[str, set[str]]:
    text = INDEX_PATH.read_text(encoding="utf-8")
    buckets = {
        "C++": set(),
        "C ABI": set(),
        "Python": set(),
        "Java": set(),
        "Rust": set(),
        "C#": set(),
    }
    section: str | None = None

    for line in text.splitlines():
        if line.startswith("## C++ Core"):
            section = "C++"
            continue
        if line.startswith("## C ABI"):
            section = "C ABI"
            continue
        if line.startswith("## Python"):
            section = "Python"
            continue
        if line.startswith("## Java"):
            section = "Java"
            continue
        if line.startswith("## Rust"):
            section = "Rust"
            continue
        if line.startswith("## C#"):
            section = "C#"
            continue
        if line.startswith("## "):
            section = None
            continue

        if section is None:
            continue

        m = re.match(r"- `(.+)`$", line.strip())
        if m:
            buckets[section].add(m.group(1))

    return buckets


def ensure_reference_links() -> list[str]:
    pages = [
        ROOT / "docs" / "reference" / "cpp-api.md",
        ROOT / "docs" / "bindings" / "c-abi.md",
        ROOT / "docs" / "bindings" / "python.md",
        ROOT / "docs" / "bindings" / "java.md",
        ROOT / "docs" / "bindings" / "rust.md",
        ROOT / "docs" / "bindings" / "csharp.md",
    ]
    missing: list[str] = []
    for page in pages:
        text = page.read_text(encoding="utf-8")
        if "api-symbol-index.md" not in text:
            missing.append(str(page.relative_to(ROOT)))
    return missing


def main() -> int:
    source = {
        "C++": set(extract_cpp_symbols()),
        "C ABI": set(extract_c_symbols()),
        "Python": flatten(extract_python_symbols()),
        "Java": flatten(extract_java_symbols()),
        "Rust": flatten(extract_rust_symbols()),
        "C#": flatten(extract_csharp_symbols()),
    }
    documented = parse_index_symbols()

    status = 0
    print("API documentation coverage report:")
    for key in ["C++", "C ABI", "Python", "Java", "Rust", "C#"]:
        src = source[key]
        doc = documented[key]
        missing = sorted(src - doc)
        extra = sorted(doc - src)
        coverage = 100.0 if not src else (len(src & doc) * 100.0 / len(src))
        print(f"- {key:6s}: {coverage:6.2f}% ({len(src & doc)}/{len(src)})")
        if missing:
            status = 1
            print(f"  missing ({len(missing)}): {', '.join(missing[:10])}")
            if len(missing) > 10:
                print("  ...")
        if extra:
            status = 1
            print(f"  stale-extra ({len(extra)}): {', '.join(extra[:10])}")
            if len(extra) > 10:
                print("  ...")

    missing_links = ensure_reference_links()
    if missing_links:
        status = 1
        print("\nMissing API symbol index links in:")
        for page in missing_links:
            print(f"- {page}")

    if status == 0:
        print("\nCoverage is 100% and symbol index is in sync with source.")
    else:
        print("\nCoverage check failed. Regenerate index and update docs.")
    return status


if __name__ == "__main__":
    sys.exit(main())
