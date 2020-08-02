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
		$(top_builddir)/test/utils/libSambadaTestUtils.a \
		-lstdc++fs

include test/integration/sambada/basicPopulationStructureIntTests/Makefile.mk
include test/integration/sambada/basicSpatialAutocorrelationIntTests/Makefile.mk
include test/integration/sambada/flotFactoriesIntTests/Makefile.mk
include test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/Makefile.mk
include test/integration/sambada/modelSelectionSavetypeBestIntTests/Makefile.mk
include test/integration/sambada/monomorphicModelsRegressionResultsIntTests/Makefile.mk
include test/integration/sambada/noExtensionsToFilenamesIntTests/Makefile.mk
include test/integration/sambada/noNewLinesEndOfFilesIntTests/Makefile.mk
include test/integration/sambada/regressionResultsIntTests/Makefile.mk
include test/integration/sambada/resultFilesLocationIntTests/Makefile.mk
include test/integration/sambada/splitInputFilesIntTests/Makefile.mk
include test/integration/sambada/spatialAutocorrelationResultsIntTests/Makefile.mk
include test/integration/sambada/storeyHistogramsIntTests/Makefile.mk
include test/integration/sambada/storeyHistogramsScoreThresholdIntTests/Makefile.mk
include test/integration/sambada/wrongNumberOfIndividualsIntTests/Makefile.mk

## Listing tests ##
check-local-test-integration-sambada: ${sambada_integration_tests}
	-${sambada_integration_tests} -l
	echo "Ignored error: The return value actually counts the numbers of tests."

## Cleaning ##
clean-local-test-integration-sambada:
	$(call clean_extra_generated_files, test/integration/sambada/)
