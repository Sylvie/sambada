
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/spatialAutocorrelationResultsIntTests/spatialAutocorrelationCartesianBisquareIntTests.cpp \
		test/integration/sambada/spatialAutocorrelationResultsIntTests/spatialAutocorrelationCartesianDistanceIntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/cattle-pop-env.csv \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/cattle-pop-mark.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/param-cartesian-bisqure.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/param-cartesian-distance.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-bisquare-env.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-bisquare-env-pval.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-bisquare-mark.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-bisquare-mark-pval.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-distance-env.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-distance-env-pval.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-distance-mark.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-distance-mark-pval.txt


