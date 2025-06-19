#!/bin/bash
set -e  # Exit on any error

echo "Building project..."
qmake CONFIG+=release "TIC_TAC_TOE_FINAL - game.pro"
make

# Find the executable (Qt uses the directory name as target by default)
EXECUTABLE="TIC_TAC_TOE_FINAL - game"
if [ -f "$EXECUTABLE" ]; then
    echo "Build successful! Running executable: $EXECUTABLE"
    ./"$EXECUTABLE"
else
    echo "Error: Executable not found. Build may have failed."
    echo "Available files in current directory:"
    ls -la
    exit 1
fi