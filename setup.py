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