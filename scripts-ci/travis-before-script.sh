#!/bin/bash
set -v
mkdir -p build
if [ "${SAMBADA_BUILD_TYPE}" = "CODE_COVERAGE" ]; then
    curl -L https://codeclimate.com/downloads/test-reporter/test-reporter-latest-linux-amd64 > ./cc-test-reporter
    chmod +x ./cc-test-reporter
    cd build
    ../cc-test-reporter before-build
fi