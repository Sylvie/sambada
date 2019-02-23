
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/storeyHistogramsWithScoreThreshold/bivariateModels/storeyHistogramsWithScoreThresholdAndPopulationStructureAsFirstVariablesDim2IntTests.cpp \
		test/integration/sambada/storeyHistogramsWithScoreThreshold/bivariateModels/storeyHistogramsWithScoreThresholdAndPopulationStructureAsLastVariablesDim2IntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/bivariateModels/expected-results-bi-dim-0.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/bivariateModels/expected-results-bi-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/bivariateModels/expected-results-bi-pop-first-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/bivariateModels/expected-results-bi-pop-last-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/bivariateModels/expected-storey-histograms-with-pop-structure-as-first-variables-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/bivariateModels/expected-storey-histograms-with-pop-structure-as-last-variables-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/bivariateModels/param-with-pop-structure-as-first-variables-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/bivariateModels/param-with-pop-structure-as-last-variables-dim-2.txt
