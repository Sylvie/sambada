
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/spatialAutocorrelationResultsIntTests/spatialAutocorrelationCartesianBisquareIntTests.cpp \
		test/integration/sambada/spatialAutocorrelationResultsIntTests/spatialAutocorrelationCartesianDistanceIntTests.cpp \
		test/integration/sambada/spatialAutocorrelationResultsIntTests/spatialAutocorrelationCartesianGaussianIntTests.cpp \
		test/integration/sambada/spatialAutocorrelationResultsIntTests/spatialAutocorrelationCartesianNearestIntTests.cpp \
		test/integration/sambada/spatialAutocorrelationResultsIntTests/spatialAutocorrelationSphericalBisquareIntTests.cpp \
		test/integration/sambada/spatialAutocorrelationResultsIntTests/spatialAutocorrelationSphericalDistanceIntTests.cpp \
		test/integration/sambada/spatialAutocorrelationResultsIntTests/spatialAutocorrelationSphericalGaussianIntTests.cpp


EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/cattle-pop-env.csv \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/cattle-pop-mark.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/random-sample.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/param-cartesian-bisqure.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/param-cartesian-distance.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/param-cartesian-nearest.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/param-spherical-bisquare.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/param-random-sample-cartesian-gausian.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/param-random-sample-spherical-gausian.txt \
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
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-gaussian-mark-pval.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-nearest-env.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-nearest-env-pval.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-nearest-mark.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-cartesian-nearest-mark-pval.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-spherical-bisquare-env.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-spherical-bisquare-env-pval.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-spherical-bisquare-mark.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-spherical-bisquare-mark-pval.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-spherical-distance-env.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-spherical-distance-env-pval.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-spherical-distance-mark.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-spherical-distance-mark-pval.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-spherical-gaussian-env.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-spherical-gaussian-env-pval.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-spherical-gaussian-mark.txt \
		$(top_srcdir)/test/integration/sambada/spatialAutocorrelationResultsIntTests/expected-results-spherical-gaussian-mark-pval.txt