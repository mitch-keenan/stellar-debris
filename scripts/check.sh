#!/bin/bash
set -e
echo "=== DSP Tests ==="
make -C test clean && make -C test && ./test/run_tests
echo ""
echo "=== Plugin Build ==="
RACK_DIR=${RACK_DIR:-/Users/mitch/vcv-rack-dev/Rack-SDK} make clean && make
echo ""
echo "=== All checks passed ==="
