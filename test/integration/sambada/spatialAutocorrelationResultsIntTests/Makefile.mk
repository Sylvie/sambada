
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/spatialAutocorrelationResultsIntTests/spatialAutocorrelationCartesianBisquareIntTests.cpp \
		test/integration/sambada/spatialAutocorrelationResultsIntTests/spatialAutocorrelationCartesianDistanceIntTests.cpp \
		test/integration/sambada/spatialAutocorrelationResultsIntTests/spatialAutocorrelationCartesianGaussianIntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/cattle-pop-env.csv \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/cattle-pop-mark.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/random-sample.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/param-cartesian-bisqure.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/param-cartesian-distance.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/param-random-sample-cartesian-gausian.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-bisquare-env.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-bisquare-env-pval.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-bisquare-mark.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-bisquare-mark-pval.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-distance-env.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-distance-env-pval.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-distance-mark.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-distance-mark-pval.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-gaussian-env.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-gaussian-env-pval.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-gaussian-mark.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-gaussian-mark-pval.txt

