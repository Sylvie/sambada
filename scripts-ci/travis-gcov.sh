#!/bin/bash
set -v
echo "TODO: measuring code coverage"
pwd
whoami
ls -l ../src
ls -l src/
echo ${GCOV}
${GCOV} -r ${TRAVIS_BUILD_DIR}/src/Archiviste.cpp src/binaries_sambada-Archiviste.o
