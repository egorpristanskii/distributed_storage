# Dockerfile.ci-base

FROM ubuntu:24.04

ARG DEBIAN_FRONTEND=noninteractive

COPY ubuntu.sources.list /etc/apt/sources.list

RUN apt-get update && apt-get install -y --no-install-recommends \
    bash \
    curl \
    coreutils \
    cmake \
    clang-19 \
    clangd-19 \
    clang-format-19 \
    clang-tidy-19 \
    libc++-19-dev \
    libc++abi-19-dev \
    libclang-rt-19-dev \
    llvm-19 \
    python3-pip \
    dpkg-dev \
    fakeroot \
    lintian \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

RUN pip install conan pre-commit pytest --break-system-packages && \
    conan profile detect

WORKDIR /root/.conan2/profiles
COPY conanprofile default

WORKDIR /build

ENTRYPOINT ["/bin/bash"]
CMD ["bash"]
