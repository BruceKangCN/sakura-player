FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-24.04

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    meson-1.5 \
    qt6-base-dev \
    qt6-declarative-dev \
    libmpv-dev
