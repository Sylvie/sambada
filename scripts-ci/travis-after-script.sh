#!/bin/bash
set -v
if [ "${SAMBADA_BUILD_TYPE}" = "CODE_COVERAGE" ]; then
    pwd
    cd build
    ls -l
    ./cc-test-reporter help format-coverage
    ./cc-test-reporter format-coverage -t gcov -d
    cat coverage/codeclimate.json
    ./cc-test-reporter help upload-coverage
    ./cc-test-reporter upload-coverage -d
fi
