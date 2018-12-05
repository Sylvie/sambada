#!/bin/bash
set -v
echo "TODO: measuring code coverage"
${GCOV} ../src/Archiviste.cpp src/binaries_sambada-Archiviste.o
