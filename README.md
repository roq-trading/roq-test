# roq-test


A test suite for managing the order life cycle using Roq market gateways.

Direct third-party dependencies

* [fmt](https://github.com/fmtlib/fmt) (MIT License)
* [gflags](https://github.com/gflags/gflags) (BDS 3-Clause License)


## Operating Systems

* Linux


## Prerequisites

The library is designed to be compatible with the conda package manager.

This is one way to create a conda environment and install the required
packages

```bash
# Download Miniconda

wget -N https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh

# Install Miniconda

bash Miniconda3-latest-Linux-x86_64.sh -b -u -p ~/miniconda3

# Activate Miniconda

source ~/miniconda3/bin/activate

# Install the toolchain

conda install -y \
    git \
    cmake \
    gxx_linux-64 \
    gdb_linux-64

# Install build dependencies

conda install -y --channel https://roq-trading.com/conda/stable \
    roq-api \
    roq-logging \
    roq-client
```


## Building

```bash
# Update git submodules

git submodule update --init --recursive

# Generate makefile

cmake \
    -DCMAKE_AR="$AR" \
    -DCMAKE_RANLIB="$RANLIB" \
    -DCMAKE_NM="$NM" \
    -DCMAKE_BUILD_TYPE=Debug \
    -DBUILD_TESTING=ON \
    .

# Compile

make -j4

# Test

make test

# Install

make install
```


## Installing

A pre-compiled binary package can be downloaded from Roq's conda package
repository

```bash
conda install -y --channel https://roq-trading.com/conda/stable \
    roq-test
```

