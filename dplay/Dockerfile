FROM ubuntu:20.04

# Install build dependencies
RUN apt-get update \
    && apt-get install --no-install-recommends -y \
        mingw-w64 \
        cmake \
        make \
    && rm -rf /var/lib/apt/lists/*
