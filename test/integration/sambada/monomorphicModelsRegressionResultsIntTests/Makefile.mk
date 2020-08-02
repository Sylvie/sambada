
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/monomorphicModelsRegressionResultsIntTests/regressionResultsDim0IntTests.cpp \
		test/integration/sambada/monomorphicModelsRegressionResultsIntTests/regressionResultsDim1IntTests.cpp

EXTRA_DIST += \
 		$(top_srcdir)/test/integration/sambada/monomorphicModelsRegressionResultsIntTests/choice-env-cattle.csv \
		$(top_srcdir)/test/integration/sambada/monomorphicModelsRegressionResultsIntTests/choice-mark-cattle.txt \
		$(top_srcdir)/test/integration/sambada/monomorphicModelsRegressionResultsIntTests/expected-results-cattle-dim-0.txt \
		$(top_srcdir)/test/integration/sambada/monomorphicModelsRegressionResultsIntTests/expected-results-cattle-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/monomorphicModelsRegressionResultsIntTests/param-dim-0.txt \
		$(top_srcdir)/test/integration/sambada/monomorphicModelsRegressionResultsIntTests/param-dim-1.txt
