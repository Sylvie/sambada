#!/bin/bash
set -v
if [ "${SAMBADA_BUILD_TYPE}" = "CODE_COVERAGE" ]; then
    ./cc-test-reporter after-build --exit-code $TRAVIS_TEST_RESULT
fi