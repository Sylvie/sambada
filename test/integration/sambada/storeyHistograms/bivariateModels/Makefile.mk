
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/storeyHistograms/bivariateModels/storeyHistogramsWithPopulationStructureAsFirstVariablesDim2IntTests.cpp \
		test/integration/sambada/storeyHistograms/bivariateModels/storeyHistogramsWithPopulationStructureAsLastVariablesDim2IntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/bivariateModels/expected-storey-histograms-with-pop-structure-as-first-variables-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/bivariateModels/expected-storey-histograms-with-pop-structure-as-last-variables-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/bivariateModels/param-with-pop-structure-as-first-variables-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/bivariateModels/param-with-pop-structure-as-last-variables-dim-2.txt
