#!/bin/bash
set -v
if [ "${SAMBADA_BUILD_TYPE}" = "CODE_COVERAGE" ]; then
    pwd
    cd build
    ls -l
    ../cc-test-reporter after-build      \
        --exit-code $TRAVIS_TEST_RESULT \
        -p "/home/travis/build/Sylvie/sambada/build/" \
        -t gcov
fi