# Custom dplay.dll 

We make our own "dplay.dll" so we can hijack our way into the process memory space.

## Dockerfile

This a cross-compile build environment used by CI (and for building locally, see below).

    docker login registry.gitlab.com
    docker build -t registry.gitlab.com/moralbacteria/diablo-prdemo-patches/dplay_build .
    docker push registry.gitlab.com/moralbacteria/diablo-prdemo-patches/dplay_build

This image is hosted on this repo: https://gitlab.com/moralbacteria/diablo-prdemo-patches/container_registry

## Building with Docker

The project can build with MinGW. This means we can dockerize it!

First, start the docker image with the source volume mounted

    docker build -t registry.gitlab.com/moralbacteria/diablo-prdemo-patches/dplay_build .
    docker run -v ${PWD}:/root/dplay -it registry.gitlab.com/moralbacteria/diablo-prdemo-patches/dplay_build bash

Then, do the actual build

    cd /root/dplay
    mkdir build
    cd build
    cmake .. -DCMAKE_TOOLCHAIN_FILE=../mingw.toolchain.cmake
    make

## What's with fake ordinals?

`DIABLO.EXE` imports two functions from the real dplay.dll:

  * DirectPlayCreate (ordinal 1)
  * DirectPlayEnumerateA (ordinal 2)

The game will complain if these ordinals are missing. However, we don't actually implement the DirectPlay API. So we provide fake functions.
