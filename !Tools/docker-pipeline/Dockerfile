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
        g++ \
        git \
        libbz2-dev \
        make \
        p7zip \
        wget \
        zlib1g-dev

# Download and extract prdemo to /root/DIABLO
WORKDIR /root
RUN wget https://diablo-evolution.net/files/prereleasedemo.7z \
    && 7zr x prereleasedemo.7z

# Clone, build, and install StormLib to tool staging directory
WORKDIR /root
RUN git clone https://github.com/ladislav-zezula/StormLib.git -b v9.20

WORKDIR /root/StormLib
RUN mkdir build \
    && cd build \
    && cmake .. -DCMAKE_INSTALL_PREFIX=/root/tools \
    && make install

# Clone, build, and install mpqadd to tool staging directory
WORKDIR /root
RUN git clone https://gitlab.com/moralbacteria/diablo-prdemo-patches.git

WORKDIR /root/diablo-prdemo-patches/!Tools/mpqadd
RUN mkdir build \
    && cd build \
    && STORM_ROOT=/root/tools cmake .. -DCMAKE_INSTALL_PREFIX=/root/tools \
    && make install

# Clone, build, and install open-vcdiff to tool staging directory
WORKDIR /root
RUN git clone https://github.com/google/open-vcdiff --recursive \
    && cd open-vcdiff \
    && git checkout 868f459a8d815125c2457f8c74b12493853100f9

WORKDIR /root/open-vcdiff
RUN mkdir build \
    && cd build \
    && cmake .. -DCMAKE_INSTALL_PREFIX=/root/tools \
    && make install

#
# Final image
#

FROM ubuntu:20.04

COPY --from=build /root/tools/ /usr/
COPY --from=build /root/DIABLO /root/DIABLO
