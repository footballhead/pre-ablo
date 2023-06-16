# CI Docker Image for host and package

Used in CI to build diabutil then package things into the final zip

## Building the Image

    docker login registry.gitlab.com
    docker build -t registry.gitlab.com/moralbacteria/diablo-prdemo-patches/host_build_and_package .
    docker push registry.gitlab.com/moralbacteria/diablo-prdemo-patches/host_build_and_package

This image is hosted on this repo: https://gitlab.com/moralbacteria/diablo-prdemo-patches/container_registry

For help logging into the GitLab registry: https://docs.gitlab.com/ee/user/packages/container_registry/#authenticate-with-the-container-registry

## Using the Docker Image

Start the Docker image with the source volume mounted

    cd docker/host_build_and_package
    docker build -t registry.gitlab.com/moralbacteria/diablo-prdemo-patches/host_build_and_package .
    cd ../..
    docker run -v ${PWD}:/root/diablo-prdemo-patches -it registry.gitlab.com/moralbacteria/diablo-prdemo-patches/host_build_and_package bash

This mounts the source to `/root/diablo-prdemo-patches` for you to do as you please
