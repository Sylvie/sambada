#!/bin/bash
set -v
echo "TODO: measuring code coverage"
pwd
cd ${TRAVIS_BUILD_DIR}
${GCOV} -r src/Archiviste.cpp -o build/src/binaries_sambada-Archiviste.o -s "../"
