
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/monomorphicModelsRegressionResultsIntTests/regressionResultsDim0IntTests.cpp \
		test/integration/sambada/monomorphicModelsRegressionResultsIntTests/regressionResultsDim0IntTests-marqueur-absent-savetype-real.cpp \
		test/integration/sambada/monomorphicModelsRegressionResultsIntTests/regressionResultsDim1IntTests.cpp \
		test/integration/sambada/monomorphicModelsRegressionResultsIntTests/regressionResultsDim2IntTests.cpp \
		test/integration/sambada/monomorphicModelsRegressionResultsIntTests/regressionResultsDim3IntTests.cpp \
		test/integration/sambada/monomorphicModelsRegressionResultsIntTests/regressionResultsDim3IntTests-marqueur-absent.cpp \
		test/integration/sambada/monomorphicModelsRegressionResultsIntTests/regressionResultsDim3IntTests-saveTypeReal.cpp

EXTRA_DIST += \
 		$(top_srcdir)/test/integration/sambada/monomorphicModelsRegressionResultsIntTests/choice-env-cattle.csv \
		$(top_srcdir)/test/integration/sambada/monomorphicModelsRegressionResultsIntTests/choice-mark-cattle.txt \
		$(top_srcdir)/test/integration/sambada/monomorphicModelsRegressionResultsIntTests/expected-results-cattle-dim-0.txt \
		$(top_srcdir)/test/integration/sambada/monomorphicModelsRegressionResultsIntTests/expected-results-cattle-dim-0-marqueur-absent-savetype-real.txt \
		$(top_srcdir)/test/integration/sambada/monomorphicModelsRegressionResultsIntTests/expected-results-cattle-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/monomorphicModelsRegressionResultsIntTests/expected-results-cattle-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/monomorphicModelsRegressionResultsIntTests/expected-results-cattle-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/monomorphicModelsRegressionResultsIntTests/expected-results-cattle-dim-3-marqueur-absent.txt \
		$(top_srcdir)/test/integration/sambada/monomorphicModelsRegressionResultsIntTests/param-dim-0.txt \
		$(top_srcdir)/test/integration/sambada/monomorphicModelsRegressionResultsIntTests/param-dim-0-marqueur-absent-savetype-real.txt \
		$(top_srcdir)/test/integration/sambada/monomorphicModelsRegressionResultsIntTests/param-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/monomorphicModelsRegressionResultsIntTests/param-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/monomorphicModelsRegressionResultsIntTests/param-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/monomorphicModelsRegressionResultsIntTests/param-dim-3-marqueur-absent.txt \
		$(top_srcdir)/test/integration/sambada/monomorphicModelsRegressionResultsIntTests/param-dim-3-savetype-real.txt