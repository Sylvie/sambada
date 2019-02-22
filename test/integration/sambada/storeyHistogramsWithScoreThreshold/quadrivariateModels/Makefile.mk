
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/storeyHistogramsWithScoreThreshold/quadrivariateModels/storeyHistogramsWithScoreThresholdAndPopulationStructureAsFirstVariablesDim4IntTests.cpp \
		test/integration/sambada/storeyHistogramsWithScoreThreshold/quadrivariateModels/storeyHistogramsWithScoreThresholdAndPopulationStructureAsLastVariablesDim4IntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/quadrivariateModels/expected-storey-histograms-with-pop-structure-as-first-variables-dim-4.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/quadrivariateModels/expected-storey-histograms-with-pop-structure-as-last-variables-dim-4.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/quadrivariateModels/param-with-pop-structure-as-first-variables-dim-4.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/quadrivariateModels/param-with-pop-structure-as-last-variables-dim-4.txt
