
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/basicSpatialAutocorrelationIntTests/basicSpatialAutocorrelationIntTests.cpp \
		test/integration/sambada/basicSpatialAutocorrelationIntTests/basicSpatialAutocorrelationMissingValuesCartesianBisquareIntTests.cpp \
		test/integration/sambada/basicSpatialAutocorrelationIntTests/basicSpatialAutocorrelationMissingValuesCartesianNearestIntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/basicSpatialAutocorrelationIntTests/param-random-sample.txt \
		$(top_srcdir)/test/integration/sambada/basicSpatialAutocorrelationIntTests/param-random-sample-missing-values-bisquare.txt \
		$(top_srcdir)/test/integration/sambada/basicSpatialAutocorrelationIntTests/param-random-sample-missing-values-nearest.txt \
		$(top_srcdir)/test/integration/sambada/basicSpatialAutocorrelationIntTests/random-sample.txt \
		$(top_srcdir)/test/integration/sambada/basicSpatialAutocorrelationIntTests/random-sample-missing-values.txt \
		$(top_srcdir)/test/integration/sambada/basicSpatialAutocorrelationIntTests/expected-results-random-sample-AS-Env.txt \
		$(top_srcdir)/test/integration/sambada/basicSpatialAutocorrelationIntTests/expected-results-random-sample-AS-Env-pVal.txt \
		$(top_srcdir)/test/integration/sambada/basicSpatialAutocorrelationIntTests/expected-results-random-sample-AS-Mark.txt \
		$(top_srcdir)/test/integration/sambada/basicSpatialAutocorrelationIntTests/expected-results-random-sample-AS-Mark-pVal.txt \
		$(top_srcdir)/test/integration/sambada/basicSpatialAutocorrelationIntTests/expected-results-random-sample-missing-values-bisquare-AS-Env.txt \
		$(top_srcdir)/test/integration/sambada/basicSpatialAutocorrelationIntTests/expected-results-random-sample-missing-values-bisquare-AS-Env-pVal.txt \
		$(top_srcdir)/test/integration/sambada/basicSpatialAutocorrelationIntTests/expected-results-random-sample-missing-values-nearest-AS-Env.txt \
		$(top_srcdir)/test/integration/sambada/basicSpatialAutocorrelationIntTests/expected-results-random-sample-missing-values-nearest-AS-Env-pVal.txt
