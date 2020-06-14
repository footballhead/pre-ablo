# Docker Distribution Pipeline

This Dockerfile is used as the build environment for the pipeline. It has some tools and an extracted version of the PR demo ready to go.

	docker login registry.gitlab.com
    docker build -t registry.gitlab.com/moralbacteria/diablo-prdemo-patches .
    docker push registry.gitlab.com/moralbacteria/diablo-prdemo-patches

This image is hosted on this repo: https://gitlab.com/moralbacteria/diablo-prdemo-patches/container_registry