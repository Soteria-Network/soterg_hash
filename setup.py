from setuptools import setup, Extension
from pathlib import Path

# Read long description from README
this_directory = Path(__file__).parent
long_description = (this_directory / "README.md").read_text(encoding="utf-8")

soterg_hash_module = Extension(
    "soterg_hash",
    sources=[
        "soterg_module.c",
        "soterg.c",
        "sha3/blake.c",
        "sha3/groestl.c",
        "sha3/jh.c",
        "sha3/keccak.c",
        "sha3/skein.c",
        "sha3/cubehash.c",
        "sha3/echo.c",
        "sha3/luffa.c",
        "sha3/sha2.c",
        "sha3/simd.c",
        "sha3/hamsi.c",
        "sha3/hamsi_helper.c",
        "sha3/shabal.c",
         "sha3/sha2big.c",
    ],
    include_dirs=[".", "./sha3"],
)

setup(
    name="soteria_soterg_hash",
    version="1.0.0",
    description="Python 3 bindings for SoterG hashing PoW",
    long_description=long_description,
    long_description_content_type="text/markdown",
    author="random.zebra",
    maintainer="The Soteria Core Developers",
    url="https://github.com/Soteria-Network/soterg_hash",
    project_urls={
        "Bug Tracker": "https://github.com/Soteria-Network/soterg_hash/issues",
        "Source Code": "https://github.com/Soteria-Network/soterg_hash",
    },
    python_requires=">=3.8",
    ext_modules=[soterg_hash_module],
)
