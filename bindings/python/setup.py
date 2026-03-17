#!/usr/bin/env python3
"""
Setup script for pypnf - Point and Figure Chart Library Python bindings.

Build modes:
  1. Standalone (bundles C++ sources): pip install .
  2. System library (links to installed libpnf): pip install . --config-settings=--build-option=--use-system-lib
"""

import os
import shutil
import subprocess
import sys
from pathlib import Path

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from setuptools.command.sdist import sdist as _sdist


def read_project_version() -> str:
    """Read the canonical project version from the repo root VERSION file."""
    env_version = os.environ.get("PNF_VERSION", "").strip()
    if env_version:
        return env_version

    candidates = [
        Path(__file__).resolve().parent / "VERSION",
        Path(__file__).resolve().parents[2] / "VERSION",
    ]
    for version_file in candidates:
        if version_file.exists():
            return version_file.read_text(encoding="utf-8").strip()

    raise FileNotFoundError(
        "Could not locate VERSION file. Set PNF_VERSION or provide bindings/python/VERSION."
    )


def read_readme() -> str:
    """Read the package README shown on PyPI."""
    return (Path(__file__).resolve().parent / "README.md").read_text(encoding="utf-8")


def find_cmake_source_dir() -> str:
    """
    Resolve the CMake project root.

    Supported layouts:
      - Repository checkout: bindings/python/setup.py with root at ../../..
      - Source distribution: setup.py at archive root with bundled core sources
      - Explicit override via PNF_SOURCE_DIR
    """
    env_source = os.environ.get("PNF_SOURCE_DIR", "").strip()
    candidates = []
    if env_source:
        candidates.append(Path(env_source).resolve())

    this_dir = Path(__file__).resolve().parent
    candidates.extend([
        this_dir,
        this_dir.parent,
        this_dir.parent.parent,
        this_dir.parent.parent.parent,
    ])

    for candidate in candidates:
        if (
            (candidate / "CMakeLists.txt").exists()
            and (candidate / "sources" / "pnf" / "chart.cpp").exists()
            and (candidate / "bindings" / "python" / "CMakeLists.txt").exists()
        ):
            return str(candidate)

    raise FileNotFoundError(
        "Unable to locate CMake source root. Set PNF_SOURCE_DIR to the project root."
    )


class PnFSdist(_sdist):
    """Create an sdist that includes the C++ core sources needed by CMake."""

    CORE_FILES = [
        "CMakeLists.txt",
        "VERSION",
        "LICENSE",
        "bindings/python/CMakeLists.txt",
        "bindings/python/pnf_python.cpp",
    ]

    CORE_DIRS = [
        "cmake",
        "headers",
        "sources",
        "bindings/c",
    ]

    def make_release_tree(self, base_dir, files):
        super().make_release_tree(base_dir, files)

        setup_dir = Path(__file__).resolve().parent
        repo_root = setup_dir.parents[1]
        if not (repo_root / "CMakeLists.txt").exists():
            return

        base_path = Path(base_dir)

        for rel_file in self.CORE_FILES:
            src = repo_root / rel_file
            if src.exists():
                dst = base_path / rel_file
                dst.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(src, dst)

        for rel_dir in self.CORE_DIRS:
            src_dir = repo_root / rel_dir
            if src_dir.exists():
                dst_dir = base_path / rel_dir
                if dst_dir.exists():
                    shutil.rmtree(dst_dir)
                shutil.copytree(src_dir, dst_dir)


class CMakeExtension(Extension):
    """Extension that uses CMake for building."""
    def __init__(self, name, sourcedir=""):
        super().__init__(name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    """Build extension using CMake."""

    def build_extension(self, ext):
        if not isinstance(ext, CMakeExtension):
            super().build_extension(ext)
            return

        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))

        # Ensure output directory exists
        if not os.path.exists(extdir):
            os.makedirs(extdir)

        # CMake configuration
        cmake_args = [
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}",
            f"-DPNF_PYTHON_OUTPUT_DIR={extdir}",
            f"-DPYTHON_EXECUTABLE={sys.executable}",
            "-DPNF_BUILD_PYTHON=ON",
            "-DPNF_BUILD_TESTS=OFF",
            "-DPNF_BUILD_EXAMPLES=OFF",
            "-DPNF_BUILD_VIEWER=OFF",
            "-DPNF_BUILD_JAVA=OFF",
            "-DPNF_BUILD_RUST=OFF",
        ]

        # Build type
        cfg = "Debug" if self.debug else "Release"
        cmake_args.append(f"-DCMAKE_BUILD_TYPE={cfg}")

        build_args = ["--config", cfg]

        # Parallel build
        if "CMAKE_BUILD_PARALLEL_LEVEL" not in os.environ:
            build_args += ["--", "-j4"]

        # Build directory
        build_temp = os.path.join(self.build_temp, ext.name)
        if not os.path.exists(build_temp):
            os.makedirs(build_temp)

        # Run CMake
        subprocess.check_call(["cmake", ext.sourcedir] + cmake_args, cwd=build_temp)
        subprocess.check_call(["cmake", "--build", ".", "--target", "pypnf"] + build_args, cwd=build_temp)


# Alternative: pybind11 direct build (simpler but requires pre-built libpnf)
def get_pybind11_extension():
    """Create extension using pybind11 directly (requires system libpnf)."""
    try:
        import pybind11
        pybind11_include = pybind11.get_include()
    except ImportError:
        pybind11_include = ""

    # Find library paths
    root_directory = Path(__file__).parent.parent.parent
    include_dir = root_directory / "include"

    return Extension(
        "pypnf",
        sources=["pnf_python.cpp"],
        include_dirs=[
            str(include_dir),
            pybind11_include,
        ],
        libraries=["pnf"],
        language="c++",
        extra_compile_args=["-std=c++20", "-O3", "-Wall"],
    )


# Determine build mode
USE_CMAKE = True  # Set to False if you want direct pybind11 build with system library

if USE_CMAKE:
    # CMake build bundles everything
    ext_modules = [CMakeExtension("pypnf", sourcedir=find_cmake_source_dir())]
    cmdclass = {"build_ext": CMakeBuild, "sdist": PnFSdist}
else:
    # Direct pybind11 build requires pre-installed libpnf
    ext_modules = [get_pybind11_extension()]
    cmdclass = {"sdist": PnFSdist}


setup(
    name="pnf-chart-system",
    version=read_project_version(),
    author="Gregorian Rayne",
    author_email="gregorianrayne09@gmail.com",
    description="Point and Figure Chart Library - Python bindings",
    long_description=read_readme(),
    long_description_content_type="text/markdown",
    url="https://github.com/gregorian-09/pnf-chart-system",
    project_urls={
        "Documentation": "https://github.com/gregorian-09/pnf-chart-system/tree/master/docs",
        "API Reference": "https://github.com/gregorian-09/pnf-chart-system/blob/master/docs/bindings/python.md",
        "Changelog": "https://github.com/gregorian-09/pnf-chart-system/blob/master/CHANGELOG.md",
        "Issues": "https://github.com/gregorian-09/pnf-chart-system/issues",
        "Source": "https://github.com/gregorian-09/pnf-chart-system",
    },
    ext_modules=ext_modules,
    py_modules=["pypnf_dashboard"],
    cmdclass=cmdclass,
    python_requires=">=3.8",
    keywords="point-and-figure, charting, technical-analysis, trading, indicators",
    classifiers=[
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "Intended Audience :: Financial and Insurance Industry",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Python :: 3.12",
    ],
)
