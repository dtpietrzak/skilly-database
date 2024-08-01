#!/bin/bash

# Output binary
OUTPUT_DIR="build"
OUTPUT="$OUTPUT_DIR/database"

# Install dependencies
echo "Installing dependencies..."
sudo apt-get update
sudo apt-get install -y clang libssl-dev libuv1-dev

# Create build directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Find all source files
SRCS=$(find . -name "*.c" -not -path "./build/*")

# Compile the project
clang -o "$OUTPUT" $SRCS \
  -I./utils -I./requests -I./server \
  -I/usr/include/openssl \
  -I/usr/include/libuv \
  -L/usr/lib -lssl -lcrypto \
  -L/usr/lib -luv

echo "Build completed: $OUTPUT"
