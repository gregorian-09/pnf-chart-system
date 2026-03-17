#!/usr/bin/env python3
"""Bump project version across core and bindings in one command."""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def update_regex(path: Path, pattern: str, repl: str, *, count: int = 1) -> None:
    raw = path.read_bytes().decode("utf-8")
    newline = "\r\n" if "\r\n" in raw else "\n"
    text = raw.replace("\r\n", "\n")
    new_text, n = re.subn(pattern, repl, text, count=count, flags=re.M)
    if n != count:
        raise RuntimeError(f"Failed to update {path} with pattern: {pattern}")
    with path.open("w", encoding="utf-8", newline="") as f:
        f.write(new_text.replace("\n", newline))


def semver(value: str) -> str:
    if not re.fullmatch(r"\d+\.\d+\.\d+", value):
        raise argparse.ArgumentTypeError("Version must be MAJOR.MINOR.PATCH (e.g. 0.2.1)")
    return value


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("version", type=semver, help="Target semantic version (MAJOR.MINOR.PATCH)")
    args = parser.parse_args()

    major, minor, patch = args.version.split(".")

    (ROOT / "VERSION").write_text(f"{args.version}\n", encoding="utf-8")

    update_regex(
        ROOT / "CMakeLists.txt",
        r"^project\(\s*pnf\s+VERSION\s+\d+\.\d+\.\d+(\s+LANGUAGES\s+CXX\s+C\s*)\)$",
        rf"project(pnf VERSION {args.version}\1)",
    )

    update_regex(
        ROOT / "headers" / "pnf" / "version.hpp",
        r"^#define PNF_VERSION_MAJOR \d+$",
        f"#define PNF_VERSION_MAJOR {major}",
    )
    update_regex(
        ROOT / "headers" / "pnf" / "version.hpp",
        r"^#define PNF_VERSION_MINOR \d+$",
        f"#define PNF_VERSION_MINOR {minor}",
    )
    update_regex(
        ROOT / "headers" / "pnf" / "version.hpp",
        r"^#define PNF_VERSION_PATCH \d+$",
        f"#define PNF_VERSION_PATCH {patch}",
    )
    update_regex(
        ROOT / "headers" / "pnf" / "version.hpp",
        r'^#define PNF_VERSION_STRING "\d+\.\d+\.\d+"$',
        f'#define PNF_VERSION_STRING "{args.version}"',
    )

    update_regex(
        ROOT / "bindings" / "java" / "pom.xml",
        r"(<version>)\d+\.\d+\.\d+(</version>)",
        rf"\g<1>{args.version}\g<2>",
    )

    update_regex(
        ROOT / "bindings" / "rust" / "Cargo.toml",
        r'^version = "\d+\.\d+\.\d+"$',
        f'version = "{args.version}"',
    )

    update_regex(
        ROOT / "bindings" / "csharp" / "PnF.csproj",
        r"(<Version>)\d+\.\d+\.\d+(</Version>)",
        rf"\g<1>{args.version}\g<2>",
    )

    print(f"Bumped project version to {args.version}")
    print("Next steps:")
    print("- Update CHANGELOG.md [Unreleased] and release section.")
    print("- Run: python3 tools/check_versions.py")
    print("- Run: python3 tools/check_docs_coverage.py")
    return 0


if __name__ == "__main__":
    sys.exit(main())
