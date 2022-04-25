#!/bin/bash
set -ev
make igb
make lib
make daemons_all
make examples_all
make avtp_pipeline
mkdir build
cd build
cmake .. -G "Unix Makefiles"
make
export ARGS=--output-on-failure
make test
cd ../daemons/gptp/doc
mkdir build
cd build
cmake ..
make doc
