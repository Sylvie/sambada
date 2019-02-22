
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/storeyHistogramsWithScoreThreshold/trivariateModels/storeyHistogramsWithScoreThresholdDim3IntTests.cpp \
		test/integration/sambada/storeyHistogramsWithScoreThreshold/trivariateModels/storeyHistogramsWithScoreThresholdAndPopulationStructureAsFirstVariablesDim3IntTests.cpp \
		test/integration/sambada/storeyHistogramsWithScoreThreshold/trivariateModels/storeyHistogramsWithScoreThresholdAndPopulationStructureAsLastVariablesDim3IntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/trivariateModels/expected-storey-histograms-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/trivariateModels/expected-storey-histograms-with-pop-structure-as-first-variables-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/trivariateModels/expected-storey-histograms-with-pop-structure-as-last-variables-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/trivariateModels/param-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/trivariateModels/param-with-pop-structure-as-first-variables-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/trivariateModels/param-with-pop-structure-as-last-variables-dim-3.txt
