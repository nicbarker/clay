#!/usr/bin/bash
REPO_ROOT=$(realpath $(dirname $(dirname $0)))

# Generate odin bindings
python $REPO_ROOT/generator/cli.py $REPO_ROOT/clay.h --output-dir $REPO_ROOT/bindings/odin/clay-odin --generator odin --verbose