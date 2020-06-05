# Docker Distribution Pipeline

This Dockerfile is used as the build environment for the pipeline. It has some tools and an extracted version of the PR demo ready to go.

    docker build -t moralbacteria/diablo-prdemo-patches .
    docker push moralbacteria/diablo-prdemo-patches:latest

This image is hosted on dockerhub: https://hub.docker.com/repository/docker/moralbacteria/diablo-prdemo-patches