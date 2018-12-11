#!/usr/bin/env bash
CC=gcc-7
CXX=g++-7
GCOV=gcov-7
MAKE=make

which lcov 1>/dev/null 2>&1
if [ $? != 0 ]
then
    echo "You need to have lcov installed in order to generate the test coverage report"
    exit 1
fi

if [ ! $DIST_ARCHIVES ]
then
    echo "You need to provide the archive name by running DIST_ARCHIVES=\"ARCHIVE_NAME\" $0"
    exit 1
fi

tar zxf $DIST_ARCHIVES.tar.gz
srcdir=${DIST_ARCHIVES}
cd $srcdir

# Reconfigure with gcov support
CC=${CC} CXX=${CXX} CXXFLAGS="-g -O0 --coverage" CFLAGS="-g -O0 --coverage" ./configure --disable-manual

# Generate gcov output
${MAKE}

# Generate html report
# Total coverage
lcov --base-directory . --directory . --zerocounters -q
lcov --base-directory . --directory . -c -i -o app_base.info --gcov-tool ${GCOV}
lcov --extract app_base.info "*sambada*/src/*" -o app_base.info # only extract source code coverage
${MAKE} check
lcov --base-directory . --directory . -c -o app_test.info --gcov-tool ${GCOV}
lcov --extract app_test.info "*sambada*/src/*" -o app_test.info # only extract source code coverage
lcov -a app_base.info -a app_test.info -o app_total.info
rm -rf ../test_coverage/total
genhtml -o ../test_coverage/total -t "Total test coverage" --num-spaces 4 app_total.info

# Total coverage
lcov --base-directory . --directory . --zerocounters -q
lcov --base-directory . --directory . -c -i -o app_base.info --gcov-tool ${GCOV}
lcov --extract app_base.info "*sambada*/src/*" -o app_base.info # only extract source code coverage
test/unit/SambadaUnitTests
lcov --base-directory . --directory . -c -o app_test.info --gcov-tool ${GCOV}
lcov --extract app_test.info "*sambada*/src/*" -o app_test.info # only extract source code coverage
lcov -a app_base.info -a app_test.info -o app_total.info
rm -rf ../test_coverage/unit
genhtml -o ../test_coverage/unit -t "Unit test coverage" --num-spaces 4 app_total.info

# Integration tests coverage
lcov --base-directory . --directory . --zerocounters -q
lcov --base-directory . --directory . -c -i -o app_base.info --gcov-tool ${GCOV}
lcov --extract app_base.info "*sambada*/src/*" -o app_base.info # only extract source code coverage
test/integration/SambadaIntegrationTests
lcov --base-directory . --directory . -c -o app_test.info --gcov-tool ${GCOV}
lcov --extract app_test.info "*sambada*/src/*" -o app_test.info # only extract source code coverage
lcov -a app_base.info -a app_test.info -o app_total.info
rm -rf ../test_coverage/integration
genhtml -o ../test_coverage/integration -t "Integration test coverage" --num-spaces 4 app_total.info

# Clean work space
cd .. && rm -rf $srcdir
