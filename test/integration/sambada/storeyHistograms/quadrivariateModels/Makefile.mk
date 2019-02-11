
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/storeyHistograms/quadrivariateModels/storeyHistogramsWithPopulationStructureAsFirstVariablesDim4IntTests.cpp \
		test/integration/sambada/storeyHistograms/quadrivariateModels/storeyHistogramsWithPopulationStructureAsLastVariablesDim4IntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/quadrivariateModels/expected-storey-histograms-with-pop-structure-as-first-variables-dim-4.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/quadrivariateModels/expected-storey-histograms-with-pop-structure-as-last-variables-dim-4.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/quadrivariateModels/param-with-pop-structure-as-first-variables-dim-4.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/quadrivariateModels/param-with-pop-structure-as-last-variables-dim-4.txt
