#
# Builder image
#

FROM ubuntu:20.04 AS build

# Install build dependencies
# noninteractive because tzdata wants input
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update \
    && apt-get install --no-install-recommends -y \
        ca-certificates \
        cmake \
        curl \
        g++ \
        git \
        libbz2-dev \
        make \
        p7zip \
        zlib1g-dev \
        unzip

# Download and extract prdemo to /root/DIABLO
WORKDIR /root
RUN curl -L "https://drive.google.com/uc?export=download&id=1E-AC21QMiOao8hVun-McROsfYBeEW20y" > prereleasedemo.7z \
    && 7zr x prereleasedemo.7z

# Clone, build, and install StormLib to tool staging directory
WORKDIR /root
RUN git clone https://github.com/ladislav-zezula/StormLib.git -b v9.20

WORKDIR /root/StormLib
RUN mkdir build \
    && cd build \
    && cmake .. -DCMAKE_INSTALL_PREFIX=/root/tools \
    && make -j $(nproc) \
    && make install

# Clone, build, and install mpqadd to tool staging directory
WORKDIR /root
RUN git clone https://gitlab.com/moralbacteria/mpqadd.git

WORKDIR /root/mpqadd
RUN mkdir build \
    && cd build \
    && STORM_ROOT=/root/tools cmake .. -DCMAKE_INSTALL_PREFIX=/root/tools \
    && make -j $(nproc) \
    && make install

#
# Final image
#

FROM ubuntu:20.04

# `curl` and `unzip` are used by CI/CD runners
RUN apt-get update \
    && apt-get install --no-install-recommends -y \
        curl \
        unzip \
    && rm -rf /var/lib/apt/lists/*

COPY --from=build /root/tools/ /usr/
COPY --from=build /root/DIABLO /root/DIABLO
