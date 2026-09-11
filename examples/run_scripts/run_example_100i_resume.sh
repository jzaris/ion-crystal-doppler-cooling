#!/bin/bash

set -e

./bin/main_2dgaussian_nersc.exe \
    examples/input/input_100i_091026.txt \
    examples/output/output_100i_091026.txt \
    examples/output/output_100i_resume_091026.txt 
