#!/bin/bash

CC=sdcc
CFLAGS="-mmcs51" #  --iram-size 256 --xram-size 0 --code-size 8192
ARCH=stc12
INCLUDES="-I./include"
PORT=""

while [[ "$#" -gt 0 ]]; do
    case $1 in
        --port) PORT="$2"; shift ;;
        *) PROJECT_PATH="$1" ;;
    esac
    shift
done

if [ -z "$PROJECT_PATH" ]; then
    echo "Usage: $0 project_path [--port /dev/xxx]"
    echo "Example: $0 examples/01-flash --port /dev/cu.usbserial-10"
    exit 1
fi

if [ ! -d "$PROJECT_PATH" ]; then
    echo "Project path does not exist: $PROJECT_PATH"
    exit 1
fi

PROJECT_NAME=$(basename "$PROJECT_PATH")

BUILD_DIR="build/$PROJECT_NAME"
mkdir -p "$BUILD_DIR"

echo "Compiling $PROJECT_PATH/main.c ..."
$CC $CFLAGS $INCLUDES -o "$BUILD_DIR/$PROJECT_NAME.ihx" "$PROJECT_PATH/main.c"

if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

echo "Build completed successfully!"

echo "Converting to HEX format..."
packihx "$BUILD_DIR/$PROJECT_NAME.ihx" > "$BUILD_DIR/$PROJECT_NAME.hex"

if [ $? -ne 0 ]; then
    echo "HEX conversion failed!"
    exit 1
fi

if [ ! -z "$PORT" ]; then
    echo "Downloading to device on $PORT..."
    stcgal -P $ARCH -p "$PORT" "$BUILD_DIR/$PROJECT_NAME.hex"
    
    if [ $? -ne 0 ]; then
        echo "Download failed!"
        exit 1
    fi
    echo "Download completed successfully!"
fi
