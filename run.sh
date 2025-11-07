#!/bin/bash

# Set locale for emoji support
export LANG=en_US.UTF-8
export LC_ALL=en_US.UTF-8
export LC_CTYPE=en_US.UTF-8

# Check if terminal supports UTF-8
if [[ "$TERM" != *"256color"* ]]; then
    export TERM=xterm-256color
fi

# Compile and run
make clean
make
./snake