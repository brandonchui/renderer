#!/bin/bash

# Enable strict error handling
set -e

echo "Starting build process..."

# Function to handle errors
handle_error() {
    echo "An error occurred. Exiting."
    read -n 1 -s -r -p "Press any key to continue..."
    exit 1
}

# Trap errors and call handle_error
trap 'handle_error' ERR

# Configure step
echo "Configuring project with CMake..."
cmake -G Ninja -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
echo "CMake configuration completed successfully."

# Build step
echo "Building project with Ninja..."
ninja -C build
echo "Build completed successfully."

# Run step
echo "Running the executable..."
./build/bin/renderer
echo "Program executed successfully."

echo "Build process completed successfully."
read -n 1 -s -r -p "Press any key to exit..."
echo
