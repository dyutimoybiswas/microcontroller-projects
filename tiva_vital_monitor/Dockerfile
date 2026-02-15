FROM ubuntu:22.04

# Install tools
RUN apt-get update && apt-get install -y \
    build-essential \
    make \
    gcc-arm-none-eabi \
    binutils-arm-none-eabi \
    libnewlib-arm-none-eabi \
    openocd \
    && rm -rf /var/lib/apt/lists/*

# Work directory inside container
WORKDIR /src

# Copy project files
COPY . /src
