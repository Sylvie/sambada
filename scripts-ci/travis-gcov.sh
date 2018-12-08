#!/bin/bash
set -v
echo "TODO: measuring code coverage"
${GCOV} -r ../src/Archiviste.cpp -o src/binaries_sambada-Archiviste.o
