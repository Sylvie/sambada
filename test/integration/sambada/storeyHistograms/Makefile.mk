
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/storeyHistograms/storeyHistogramsDim1IntTests.cpp \
		test/integration/sambada/storeyHistograms/storeyHistogramsDim3IntTests.cpp \
		test/integration/sambada/storeyHistograms/storeyHistogramsWithSavetypeBestIntTests.cpp

EXTRA_DIST += \
 		$(top_srcdir)/test/integration/sambada/storeyHistograms/cattle-pop-env-first.csv \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/cattle-pop-mark.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/expected-storey-histograms-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/expected-storey-histograms-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/param-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/param-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/param-savetype-best.txt
