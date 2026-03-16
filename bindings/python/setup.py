#!/usr/bin/env python3
"""
Setup script for pypnf - Point and Figure Chart Library Python bindings.

Build modes:
  1. Standalone (bundles C++ sources): pip install .
  2. System library (links to installed libpnf): pip install . --config-settings=--build-option=--use-system-lib
"""

import os
import subprocess
\"\"\"Build script for the Python bindings.\"\"\"

import sys
from pathlib import Path

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext


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
            f"-DPYTHON_EXECUTABLE={sys.executable}",
            "-DPNF_BUILD_PYTHON=ON",
            "-DPNF_BUILD_TESTS=OFF",
            "-DPNF_BUILD_EXAMPLES=OFF",
            "-DPNF_BUILD_VIEWER=OFF",
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
    root_dir = str(Path(__file__).parent.parent.parent)
    ext_modules = [CMakeExtension("pypnf", sourcedir=root_dir)]
    cmdclass = {"build_ext": CMakeBuild}
else:
    # Direct pybind11 build requires pre-installed libpnf
    ext_modules = [get_pybind11_extension()]
    cmdclass = {}


setup(
    name="pypnf",
    version="2.0.0",
    author="Gregorian Rayne",
    author_email="gregorianrayne@gmail.com",
    description="Point and Figure Chart Library - Python bindings",
    long_description=open("README.md").read() if os.path.exists("README.md") else "",
    long_description_content_type="text/markdown",
    url="https://github.com/gregorian-09/pnf-chart-system",
    ext_modules=ext_modules,
    py_modules=["pypnf_dashboard"],
    cmdclass=cmdclass,
    python_requires=">=3.8",
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
