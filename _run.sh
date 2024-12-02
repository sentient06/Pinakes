#!/bin/bash

this=$(basename "$PWD")
cd ..
./clean.sh $this
./build_and_run.sh $this 1