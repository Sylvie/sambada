noinst_LIBRARIES += test/integration/utils/libSambadaIntegrationTestUtils.a

test_integration_utils_libSambadaIntegrationTestUtils_a_SOURCES = \
    test/integration/utils/SambadaIntegrationTestUtils.cpp \
    test/integration/utils/SambadaIntegrationTestUtils.h
test_integration_utils_libSambadaIntegrationTestUtils_a_CPPFLAGS = \
 	-I $(top_srcdir)/test/utils/ \
 	-Dsambada_top_source_dir=\"$(top_srcdir)\"

test_integration_utils_libSambadaIntegrationTestUtils_a_LIBADD = $(top_builddir)/test/utils/libSambadaTestUtils.a

if WINDOWS
test_integration_utils_libSambadaIntegrationTestUtils_a_SOURCES += test/integration/utils/SambadaIntegrationTestUtilsWindows.cpp
else
test_integration_utils_libSambadaIntegrationTestUtils_a_SOURCES += test/integration/utils/SambadaIntegrationTestUtilsUnix.cpp
endif

## Cleaning ##
clean-local-test-integration-utils:
	$(RM) -rf $(gcov_filename_suffixes)
