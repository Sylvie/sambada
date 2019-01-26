
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/storeyHistograms/storeyHistogramsDim1IntTests.cpp \
		test/integration/sambada/storeyHistograms/storeyHistogramsDim4IntTests.cpp

EXTRA_DIST += \
 		$(top_srcdir)/test/integration/sambada/storeyHistograms/choice-env-cattle.csv \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/choice-mark-cattle.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/expected-storey-histograms-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/expected-storey-histograms-dim-4.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/param-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/param-dim-4.txt
