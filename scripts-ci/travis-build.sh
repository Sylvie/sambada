#!/bin/bash
set -v
autoreconf -i
echo $CC $CXX
${CC} -v; ${CXX} -v
sambada_version=$(./git-version-gen .version)
mkdir -p build
cd build
../configure sambadahostsystemname=${SAMBADA_OS_NAME}
cat config.log
make pdf
if [ "${SAMBADA_BUILD_TYPE}" = "PACKAGE" ]; then
    make
else
    build-wrapper-linux-x86-64 --out-dir bw-outputs make
    ls bw-outputs
fi
make check
cp test-suite.log ../test-suite-results.log
if [ "${SAMBADA_BUILD_TYPE}" = "PACKAGE" ]; then
    binaries/sambada ../Data/Example_parameters_input_matrix.txt ../Data/input_matrix.csv
    make binary-archive
    ls -R
    make clean
    make distcheck sambadahostsystemname=${SAMBADA_OS_NAME}
else
    pwd
    source ${TRAVIS_BUILD_DIR}/scripts-ci/travis-gcov.sh
    cd ..
    sonar-scanner -X
fi
