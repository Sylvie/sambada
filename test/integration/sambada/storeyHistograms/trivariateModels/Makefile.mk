
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/storeyHistograms/trivariateModels/storeyHistogramsDim3IntTests.cpp \
		test/integration/sambada/storeyHistograms/trivariateModels/storeyHistogramsWithPopulationStructureAsFirstVariablesDim3IntTests.cpp \
		test/integration/sambada/storeyHistograms/trivariateModels/storeyHistogramsWithPopulationStructureAsLastVariablesDim3IntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/trivariateModels/expected-storey-histograms-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/trivariateModels/expected-storey-histograms-with-pop-structure-as-first-variables-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/trivariateModels/expected-storey-histograms-with-pop-structure-as-last-variables-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/trivariateModels/param-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/trivariateModels/param-with-pop-structure-as-first-variables-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/trivariateModels/param-with-pop-structure-as-last-variables-dim-3.txt
