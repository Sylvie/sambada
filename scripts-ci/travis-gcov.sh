#!/bin/bash
set -v
echo "TODO: measuring code coverage"
pwd
${GCOV} -r ../src/Archiviste.cpp -o src/binaries_sambada-Archiviste.o
cd ..
sonar-scanner -X
