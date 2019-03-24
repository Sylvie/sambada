supervision_integration_tests = test/integration/SupervisionIntegrationTests

noinst_PROGRAMS += ${supervision_integration_tests}
TESTS += ${supervision_integration_tests}

test_integration_SupervisionIntegrationTests_SOURCES = ${catch_source_files}
test_integration_SupervisionIntegrationTests_CPPFLAGS = -I $(top_srcdir)/src \
 		-I $(top_srcdir)/test/utils \
 		-I $(top_srcdir)/test/integration/utils \
 		-Dsambada_executable_extension=\"${sambada_executable_extension}\"
test_integration_SupervisionIntegrationTests_LDADD = \
		$(top_builddir)/test/integration/utils/libSambadaIntegrationTestUtils.a \
		$(top_builddir)/test/utils/libSambadaTestUtils.a \
		-lstdc++fs

include test/integration/supervision/inputFileSplittingIntTests/Makefile.mk

## Listing tests ##
check-local-test-integration-supervision: ${supervision_integration_tests}
	-${supervision_integration_tests} -l
	echo "Ignored error: The return value actually counts the numbers of tests."

## Cleaning ##
clean-local-test-integration-supervision:
	$(call clean_extra_generated_files, test/integration/supervision/)
