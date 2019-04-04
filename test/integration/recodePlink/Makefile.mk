recode_plink_integration_tests = test/integration/RecodePlinkIntegrationTests

noinst_PROGRAMS += ${recode_plink_integration_tests}
TESTS += ${recode_plink_integration_tests}

test_integration_RecodePlinkIntegrationTests_SOURCES = ${catch_source_files}
test_integration_RecodePlinkIntegrationTests_CPPFLAGS = -I $(top_srcdir)/src \
 		-I $(top_srcdir)/test/utils \
 		-I $(top_srcdir)/test/integration/utils \
 		-Dsambada_executable_extension=\"${sambada_executable_extension}\"
test_integration_RecodePlinkIntegrationTests_LDADD = \
		$(top_builddir)/test/integration/utils/libSambadaIntegrationTestUtils.a \
		$(top_builddir)/test/utils/libSambadaTestUtils.a \
		-lstdc++fs

include test/integration/recodePlink/recodeForSambadaIntTests/Makefile.mk
include test/integration/recodePlink/recodeForLFMMIntTests/Makefile.mk

## Listing tests ##
check-local-test-integration-recode-plink: ${recode_plink_integration_tests}
	-${recode_plink_integration_tests} -l
	echo "Ignored error: The return value actually counts the numbers of tests."

## Cleaning ##
clean-local-test-integration-recode-plink:
	$(call clean_extra_generated_files, test/integration/recodePlink/)
