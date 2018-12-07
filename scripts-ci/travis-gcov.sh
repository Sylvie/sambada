#!/bin/bash
set -v
echo "TODO: measuring code coverage"
pwd
whoami
ls -l ../src
ls -l src/
echo ${GCOV}
${GCOV} -r ${TRAVIS_BUILD_DIR}/src/Archiviste.cpp -o ${TRAVIS_BUILD_DIR}/build/src/binaries_sambada-Archiviste.o
