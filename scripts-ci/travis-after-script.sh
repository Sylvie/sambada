#!/bin/bash
set -v
if [ "${SAMBADA_BUILD_TYPE}" = "CODE_COVERAGE" ]; then
    pwd
    cd build
    ls -l
    ../cc-test-reporter format-coverage \
        --exit-code $TRAVIS_TEST_RESULT \
        -t gcov \
        -d
    cat coverage/codeclimate.json
    ../cc-test-reporter upload-coverage -d
fi