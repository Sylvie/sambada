
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/regressionResultsIntTests/regressionResultsDim1IntTests.cpp \
		test/integration/sambada/regressionResultsIntTests/regressionResultsDim4IntTests.cpp

EXTRA_DIST += \
 		$(top_srcdir)/test/integration/sambada/regressionResultsIntTests/choice-env-cattle.csv \
		$(top_srcdir)/test/integration/sambada/regressionResultsIntTests/choice-mark-cattle.txt \
		$(top_srcdir)/test/integration/sambada/regressionResultsIntTests/expected-results-cattle-dim-0.txt \
		$(top_srcdir)/test/integration/sambada/regressionResultsIntTests/expected-results-cattle-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/regressionResultsIntTests/expected-results-cattle-dim-4.txt \
		$(top_srcdir)/test/integration/sambada/regressionResultsIntTests/param-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/regressionResultsIntTests/param-dim-4.txt
