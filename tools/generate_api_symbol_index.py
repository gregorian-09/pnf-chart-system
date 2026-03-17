#!/usr/bin/env python3
"""Generate exhaustive API symbol index docs from source files.

This script builds a machine-verified symbol appendix so docs can stay in sync
with the implemented API surface across C++, C ABI, Python, Java, Rust, and C#.
"""

from __future__ import annotations

import re
from pathlib import Path
from typing import Dict, List

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "docs" / "reference" / "api-symbol-index.md"


def read(path: Path) -> str:
    return path.read_text(encoding="utf-8")


def clean_ws(text: str) -> str:
    return re.sub(r"\s+", " ", text).strip()


CPP_HEADERS = [
    ROOT / "headers" / "pnf" / "types.hpp",
    ROOT / "headers" / "pnf" / "box.hpp",
    ROOT / "headers" / "pnf" / "column.hpp",
    ROOT / "headers" / "pnf" / "chart.hpp",
    ROOT / "headers" / "pnf" / "indicators.hpp",
    ROOT / "headers" / "pnf" / "trendline.hpp",
    ROOT / "headers" / "pnf" / "visualization.hpp",
    ROOT / "headers" / "pnf" / "csv_loader.hpp",
    ROOT / "headers" / "pnf" / "version.hpp",
]


def extract_cpp_symbols() -> List[str]:
    symbols: set[str] = set()

    def strip_comments(src: str) -> str:
        src = re.sub(r"/\*.*?\*/", "", src, flags=re.S)
        src = re.sub(r"//.*", "", src)
        return src

    func_name_re = re.compile(r"\b([A-Za-z_][A-Za-z0-9_]*)\s*\([^;{}()]*\)\s*(?:const\s*)?(?:;|\{)")

    for hdr in CPP_HEADERS:
        text = strip_comments(read(hdr))
        lines = text.splitlines()

        # Top-level type names
        for m in re.finditer(r"\benum\s+class\s+([A-Za-z_][A-Za-z0-9_]*)", text):
            symbols.add(m.group(1))
        for m in re.finditer(r"\bstruct\s+([A-Za-z_][A-Za-z0-9_]*)\s*\{", text):
            symbols.add(m.group(1))
        for m in re.finditer(r"\bclass\s+([A-Za-z_][A-Za-z0-9_]*)\s*\{", text):
            symbols.add(m.group(1))

        in_type = False
        access = "private"
        brace_depth = 0
        in_namespace = False

        for raw in lines:
            line = raw.strip()
            if not line:
                continue

            if line.startswith("namespace ") and line.endswith("{"):
                in_namespace = True
                continue
            if in_namespace and line == "}":
                in_namespace = False
                continue

            if not in_type:
                # Detect class/struct start
                if re.search(r"\b(class|struct)\s+[A-Za-z_][A-Za-z0-9_]*\s*\{", line):
                    in_type = True
                    access = "public" if line.startswith("struct ") else "private"
                    brace_depth = line.count("{") - line.count("}")
                    continue
                # Top-level free function declaration
                m = func_name_re.search(line)
                if in_namespace and m:
                    name = m.group(1)
                    if name not in {"if", "for", "while", "switch", "return"}:
                        symbols.add(name)
                continue

            # Inside class/struct
            if line in {"public:", "private:", "protected:"}:
                access = line[:-1]
                continue

            if access == "public":
                m = func_name_re.search(line)
                if m:
                    name = m.group(1)
                    if name not in {"if", "for", "while", "switch", "return"}:
                        symbols.add(name)

            brace_depth += line.count("{") - line.count("}")
            if brace_depth <= 0:
                in_type = False
                access = "private"

    return sorted(symbols)


def extract_c_symbols() -> List[str]:
    text = read(ROOT / "bindings" / "c" / "pnf_c.hpp")
    symbols = set(re.findall(r"PNF_API\s+[A-Za-z_][A-Za-z0-9_\*\s]*\s+(pnf_[A-Za-z0-9_]+)\s*\(", text))
    return sorted(symbols)


def extract_python_symbols() -> Dict[str, List[str]]:
    text = read(ROOT / "bindings" / "python" / "pnf_python.cpp")
    lines = text.splitlines()
    groups: Dict[str, set[str]] = {"module": set()}
    current_class = "module"
    class_re = re.compile(r'py::class_<[^>]+>\(m,\s*"([A-Za-z_][A-Za-z0-9_]*)"\)')
    def_re = re.compile(r'\.def\("([A-Za-z_][A-Za-z0-9_]*)"')
    mod_def_re = re.compile(r'm\.def\("([A-Za-z_][A-Za-z0-9_]*)"')

    for line in lines:
        cm = class_re.search(line)
        if cm:
            current_class = cm.group(1)
            groups.setdefault(current_class, set())
        mm = mod_def_re.search(line)
        if mm:
            groups["module"].add(mm.group(1))
        dm = def_re.search(line)
        if dm:
            groups.setdefault(current_class, set()).add(dm.group(1))

    return {k: sorted(v) for k, v in sorted(groups.items(), key=lambda kv: kv[0].lower())}


def extract_java_symbols() -> Dict[str, List[str]]:
    base = ROOT / "bindings" / "java" / "com" / "pnf"
    out: Dict[str, List[str]] = {}
    for p in sorted(base.glob("*.java")):
        text = read(p)
        sigs = []
        # Public methods and ctors
        for m in re.finditer(
            r"public\s+(?:static\s+)?(?:[A-Za-z0-9_<>\[\], ?]+\s+)?([A-Za-z_][A-Za-z0-9_]*)\s*\(([^)]*)\)\s*\{",
            text,
        ):
            sigs.append(f"{m.group(1)}({clean_ws(m.group(2))})")
        # Public records
        for m in re.finditer(r"public\s+record\s+([A-Za-z_][A-Za-z0-9_]*)\(([^)]*)\)", text):
            sigs.append(f"record {m.group(1)}({clean_ws(m.group(2))})")
        if sigs:
            out[p.stem] = sorted(set(sigs))
    return out


def extract_rust_symbols() -> Dict[str, List[str]]:
    base = ROOT / "bindings" / "rust" / "src"
    out: Dict[str, List[str]] = {}
    for p in sorted(base.glob("*.rs")):
        text = read(p)
        sigs = []
        for m in re.finditer(r"^\s*pub\s+fn\s+([A-Za-z_][A-Za-z0-9_]*)\s*\(([^)]*)\)", text, re.M):
            sigs.append(f"{m.group(1)}({clean_ws(m.group(2))})")
        for m in re.finditer(r"^\s*pub\s+enum\s+([A-Za-z_][A-Za-z0-9_]*)", text, re.M):
            sigs.append(f"enum {m.group(1)}")
        for m in re.finditer(r"^\s*pub\s+struct\s+([A-Za-z_][A-Za-z0-9_]*)", text, re.M):
            sigs.append(f"struct {m.group(1)}")
        if sigs:
            out[p.stem] = sorted(set(sigs))
    return out


def extract_csharp_symbols() -> Dict[str, List[str]]:
    base = ROOT / "bindings" / "csharp"
    out: Dict[str, List[str]] = {}
    for p in sorted(base.glob("*.cs")):
        if p.name.endswith("Tests.cs"):
            continue
        text = read(p)
        sigs = []
        for m in re.finditer(
            r"public\s+(?:static\s+)?(?:override\s+)?(?:[A-Za-z0-9_<>\[\], ?]+\s+)?([A-Za-z_][A-Za-z0-9_]*)\s*\(([^)]*)\)",
            text,
        ):
            sigs.append(f"{m.group(1)}({clean_ws(m.group(2))})")
        for m in re.finditer(r"public\s+(?:static\s+)?[A-Za-z0-9_<>\[\], ?]+\s+([A-Za-z_][A-Za-z0-9_]*)\s*\{\s*get;", text):
            sigs.append(f"{m.group(1)} (property)")
        for m in re.finditer(r"public\s+enum\s+([A-Za-z_][A-Za-z0-9_]*)", text):
            sigs.append(f"enum {m.group(1)}")
        for m in re.finditer(r"public\s+struct\s+([A-Za-z_][A-Za-z0-9_]*)", text):
            sigs.append(f"struct {m.group(1)}")
        if sigs:
            out[p.stem] = sorted(set(sigs))
    return out


def section_list(title: str, items: List[str]) -> str:
    lines = [f"### {title}", ""]
    for item in items:
        lines.append(f"- `{item}`")
    lines.append("")
    return "\n".join(lines)


def section_map(title: str, groups: Dict[str, List[str]]) -> str:
    lines = [f"## {title}", ""]
    total = sum(len(v) for v in groups.values())
    lines.append(f"Total symbols: **{total}**")
    lines.append("")
    for key, vals in groups.items():
        lines.append(f"### `{key}`")
        lines.append("")
        for v in vals:
            lines.append(f"- `{v}`")
        lines.append("")
    return "\n".join(lines)


def main() -> None:
    cpp = extract_cpp_symbols()
    c_api = extract_c_symbols()
    py = extract_python_symbols()
    java = extract_java_symbols()
    rust = extract_rust_symbols()
    cs = extract_csharp_symbols()

    md: List[str] = []
    md.append("# API Symbol Index")
    md.append("")
    md.append("This file is generated from source and provides exhaustive symbol coverage across the project APIs.")
    md.append("")
    md.append("Do not edit manually. Regenerate with:")
    md.append("")
    md.append("```bash")
    md.append("python3 tools/generate_api_symbol_index.py")
    md.append("```")
    md.append("")

    md.append(f"- C++ symbols: **{len(cpp)}**")
    md.append(f"- C ABI functions: **{len(c_api)}**")
    md.append(f"- Python symbols: **{sum(len(v) for v in py.values())}**")
    md.append(f"- Java symbols: **{sum(len(v) for v in java.values())}**")
    md.append(f"- Rust symbols: **{sum(len(v) for v in rust.values())}**")
    md.append(f"- C# symbols: **{sum(len(v) for v in cs.values())}**")
    md.append("")

    md.append("## C++ Core")
    md.append("")
    md.append(f"Total symbols: **{len(cpp)}**")
    md.append("")
    for s in cpp:
        md.append(f"- `{s}`")
    md.append("")

    md.append("## C ABI")
    md.append("")
    md.append(f"Total functions: **{len(c_api)}**")
    md.append("")
    for s in c_api:
        md.append(f"- `{s}`")
    md.append("")

    md.append(section_map("Python (`pypnf`)", py))
    md.append(section_map("Java (`com.pnf`)", java))
    md.append(section_map("Rust (`pnf`)", rust))
    md.append(section_map("C# (`PnF`)", cs))

    OUT.write_text("\n".join(md).rstrip() + "\n", encoding="utf-8")
    print(f"Wrote {OUT}")


if __name__ == "__main__":
    main()
