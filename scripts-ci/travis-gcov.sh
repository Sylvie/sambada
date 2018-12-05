#!/bin/bash
set -v
echo "TODO: measuring code coverage"
pwd
whoami
ls -l ../src
ls -l src/
${GCOV} ../src/Archiviste.cpp src/binaries_sambada-Archiviste.o
