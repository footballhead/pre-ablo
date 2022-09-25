# We used to use conanio/gcc11-ubuntu16.04 but they only package CMake 3.5... we need CMake 3.8 to use C++17
# That was the straw that broke the camels back. This lets us control our own destiny.
FROM ubuntu:22.04

# Install build and package dependencies
RUN apt-get update \
    && apt-get install --no-install-recommends -y \
        cmake \
        python3-pip \
        g++ \
        ninja-build \
        zlib1g-dev \
        libbz2-dev \
    && rm -rf /var/lib/apt/lists/* 

# Install conan
# TODO Remove image bulk (caches, etc.)
RUN pip install conan

# Setup conan
RUN conan remote add gitlab https://gitlab.com/api/v4/projects/18183220/packages/conan 
