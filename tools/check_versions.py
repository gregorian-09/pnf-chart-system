#!/usr/bin/env python3
"""Validate that all publishable artifacts use the same project version."""

from __future__ import annotations

import re
import sys
from pathlib import Path
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]


def read_text(path: Path) -> str:
    return path.read_text(encoding="utf-8")


def read_root_version() -> str:
    return read_text(ROOT / "VERSION").strip()


def cmake_version() -> str:
    text = read_text(ROOT / "CMakeLists.txt")
    m = re.search(r"project\s*\(\s*pnf\s+VERSION\s+([0-9]+\.[0-9]+\.[0-9]+)", text)
    if not m:
        raise RuntimeError("Could not parse CMake project version")
    return m.group(1)


def header_version() -> str:
    text = read_text(ROOT / "headers" / "pnf" / "version.hpp")
    ms = re.search(r"#define\s+PNF_VERSION_STRING\s+\"([0-9]+\.[0-9]+\.[0-9]+)\"", text)
    mm = re.search(r"#define\s+PNF_VERSION_MAJOR\s+([0-9]+)", text)
    mn = re.search(r"#define\s+PNF_VERSION_MINOR\s+([0-9]+)", text)
    mp = re.search(r"#define\s+PNF_VERSION_PATCH\s+([0-9]+)", text)
    if not (ms and mm and mn and mp):
        raise RuntimeError("Could not parse headers/pnf/version.hpp")
    composed = f"{mm.group(1)}.{mn.group(1)}.{mp.group(1)}"
    if composed != ms.group(1):
        raise RuntimeError(
            f"Version macro mismatch in headers/pnf/version.hpp: string={ms.group(1)} numeric={composed}"
        )
    return ms.group(1)


def python_version() -> str:
    text = read_text(ROOT / "bindings" / "python" / "setup.py")
    if re.search(r"version\s*=\s*read_project_version\s*\(", text):
        return read_root_version()
    m = re.search(r"version\s*=\s*\"([0-9]+\.[0-9]+\.[0-9]+)\"", text)
    if not m:
        raise RuntimeError("Could not parse Python binding version")
    return m.group(1)


def java_version() -> str:
    path = ROOT / "bindings" / "java" / "pom.xml"
    tree = ET.parse(path)
    root = tree.getroot()
    ns = {"m": "http://maven.apache.org/POM/4.0.0"}
    node = root.find("m:version", ns)
    if node is None or not node.text:
        raise RuntimeError("Could not parse Java pom version")
    return node.text.strip()


def rust_version() -> str:
    text = read_text(ROOT / "bindings" / "rust" / "Cargo.toml")
    package_block = re.search(r"\[package\](.*?)(\n\[|\Z)", text, flags=re.S)
    if not package_block:
        raise RuntimeError("Could not parse Rust Cargo.toml package block")
    m = re.search(r"^version\s*=\s*\"([0-9]+\.[0-9]+\.[0-9]+)\"", package_block.group(1), flags=re.M)
    if not m:
        raise RuntimeError("Could not parse Rust crate version")
    return m.group(1)


def csharp_version() -> str:
    text = read_text(ROOT / "bindings" / "csharp" / "PnF.csproj")
    m = re.search(r"<Version>([0-9]+\.[0-9]+\.[0-9]+)</Version>", text)
    if not m:
        raise RuntimeError("Could not parse C# project version")
    return m.group(1)


def main() -> int:
    expected = read_root_version()
    versions = {
        "VERSION": expected,
        "CMake project": cmake_version(),
        "C++ header macros": header_version(),
        "Python package": python_version(),
        "Java package": java_version(),
        "Rust crate": rust_version(),
        "C# package": csharp_version(),
    }

    print(f"Expected version: {expected}")
    print("")
    bad = []
    for key, value in versions.items():
        ok = value == expected
        print(f"{'OK ' if ok else 'BAD'} {key:20s} {value}")
        if not ok:
            bad.append((key, value))

    if bad:
        print("\nVersion mismatches detected:")
        for key, value in bad:
            print(f"- {key}: {value} (expected {expected})")
        return 1

    print("\nAll version sources are synchronized.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
