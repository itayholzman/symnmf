"""
setup.py for building the SymNMF Python C API extension.

This script uses setuptools to compile C source files into a Python extension
module named `symnmf_capi`. The module exposes functions for symmetric non-negative
matrix factorization (SymNMF) and related matrix operations.
"""
from setuptools import setup, Extension

module = Extension(
    "symnmf_capi",
    sources=[
        "symnmfmodule.c",
        "symnmf.c",
        "matrix_utils.c",
        "points_list_utils.c"
    ],
)

setup(
    name="symnmf_capi",
    version="1.0",
    ext_modules=[module]
)