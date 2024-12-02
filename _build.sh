#!/bin/bash

this=$(basename "$PWD")
cd ..
./clean.sh $this
./build.sh $this