#!/bin/bash

# This script automates the build and execution process.

# Exit immediately if a command exits with a non-zero status.
set -e

# Navigate to the project root directory (parent of scripts folder)
cd "$(dirname "$0")/.."

# Define the build directory
BUILD_DIR="cmake-build-debug"

# Create the build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
  echo "Creating build directory: $BUILD_DIR"
  mkdir -p "$BUILD_DIR"
fi

# Navigate into the build directory
cd "$BUILD_DIR"

# Configure the project with CMake
# This step only runs if the cache is missing or Makefiles need regeneration.
echo "Configuring project with CMake..."
cmake ..

# Build the project using the native build system (make or ninja)
echo "Building project..."
cmake --build .

# Navigate back to the root directory
cd ..

# Run the executable from the root directory
# This ensures it can find any relative paths if needed in the future.
./"$BUILD_DIR"/HashMap
