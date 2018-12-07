#!/bin/bash
set -v
if [ "${SAMBADA_BUILD_TYPE}" = "CODE_COVERAGE" ]; then
    pwd
    cd build
    ls -l
    ./cc-test-reporter format-coverage -d \
        --prefix "/home/travis/build/Sylvie/sambada/build/"
    cat coverage/codeclimate.json
    ./cc-test-reporter upload-coverage -d
fi
