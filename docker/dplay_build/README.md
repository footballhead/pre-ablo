# CI Docker Image for win32

Docker which includes a mingw-w64 toolchain for crosscompiling for Windows.

## Dockerfile

This a cross-compile build environment used by CI (and for building locally, see below).

    docker login registry.gitlab.com
    docker build -t registry.gitlab.com/moralbacteria/diablo-prdemo-patches/dplay_build .
    docker push registry.gitlab.com/moralbacteria/diablo-prdemo-patches/dplay_build

This image is hosted on this repo: https://gitlab.com/moralbacteria/diablo-prdemo-patches/container_registry

## Building with Docker

First, start the docker image with the source volume mounted

    docker build -t registry.gitlab.com/moralbacteria/diablo-prdemo-patches/dplay_build .
    docker run -v ${PWD}:/root/diablo-prdemo-patches -it registry.gitlab.com/moralbacteria/diablo-prdemo-patches/dplay_build bash

The source will be mounted as `/root/diablo-prdemo-patches`
