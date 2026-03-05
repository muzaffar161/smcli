#!/bin/bash

# Compiler
CXX=g++

# Compiler flags
CXXFLAGS="-std=c++17 -I./include -Wall -Wextra"

# Source files
SRCS=$(find ./src -name '*.cpp')

# Output directory
TARGET_DIR="bin"

# Output executable name
TARGET_NAME="smcli"
TARGET_PATH="$TARGET_DIR/$TARGET_NAME"

# Create target directory if it doesn't exist
mkdir -p $TARGET_DIR

# Build command
echo "Building $TARGET_NAME..."
$CXX $CXXFLAGS -o $TARGET_PATH $SRCS

if [ $? -eq 0 ]; then
  echo "Build successful! Executable is at '$TARGET_PATH'."
else
  echo "Build failed."
fi