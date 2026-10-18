# ==============================================================================
# Dockerfile: Linux OS Systems Programming Lab Environment
# Fundamentals of Operating Systems - Hussein Nasser
# ==============================================================================

FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    gcc \
    g++ \
    gdb \
    make \
    binutils \
    strace \
    ltrace \
    procps \
    iproute2 \
    net-tools \
    curl \
    python3 \
    python3-pip \
    valgrind \
    man-db \
    manpages-dev \
    manpages-posix-dev \
    git \
    vim \
    nano \
    less \
    tree \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

CMD ["/bin/bash"]
