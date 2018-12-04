#!/bin/bash
set -v
autoreconf -i
echo $CC $CXX
${CC} -v; ${CXX} -v
sambada_version=$(./git-version-gen .version)
mkdir build
cd build
../configure sambadahostsystemname=${SAMBADA_OS_NAME}
cat config.log
make pdf
make
make check
cp test-suite.log ../test-suite-results.log
if [ "${SAMBADA_BUILD_TYPE}" -eq "PACKAGE" ]; then
    binaries/sambada ../Data/Example_parameters_input_matrix.txt ../Data/input_matrix.csv
    ls -R
    make clean
    make distcheck sambadahostsystemname=${SAMBADA_OS_NAME}
    ls -R
    make binary-archive
    cd ..
    ls -R
else
    echo "TODO: measuring code coverage"
fi
