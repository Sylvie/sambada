sambada_integration_tests = test/integration/SambadaIntegrationTests

noinst_PROGRAMS += ${sambada_integration_tests}
TESTS += ${sambada_integration_tests}

test_integration_SambadaIntegrationTests_SOURCES = ${catch_source_files}
test_integration_SambadaIntegrationTests_CPPFLAGS = -I $(top_srcdir)/src \
 		-I $(top_srcdir)/test/utils \
 		-I $(top_srcdir)/test/integration/utils \
 		-Dsambada_executable_extension=\"${sambada_executable_extension}\"
test_integration_SambadaIntegrationTests_LDADD = \
		$(top_builddir)/test/integration/utils/libSambadaIntegrationTestUtils.a \
		$(top_builddir)/test/utils/libSambadaTestUtils.a

include test/integration/sambada/regressionresultsIntTests/Makefile.mk

## Cleaning ##
clean-local-test-integration-sambada:
	 $(RM) -rf $(addsuffix .dSYM , $(sambada_integration_tests))
