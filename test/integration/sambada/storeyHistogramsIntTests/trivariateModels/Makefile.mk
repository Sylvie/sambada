
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/storeyHistogramsIntTests/trivariateModels/storeyHistogramsDim3IntTests.cpp \
		test/integration/sambada/storeyHistogramsIntTests/trivariateModels/storeyHistogramsWithPopulationStructureAsFirstVariablesDim3IntTests.cpp \
		test/integration/sambada/storeyHistogramsIntTests/trivariateModels/storeyHistogramsWithPopulationStructureAsLastVariablesDim3IntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsIntTests/trivariateModels/expected-storey-histograms-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsIntTests/trivariateModels/expected-storey-histograms-with-pop-structure-as-first-variables-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsIntTests/trivariateModels/expected-storey-histograms-with-pop-structure-as-last-variables-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsIntTests/trivariateModels/param-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsIntTests/trivariateModels/param-with-pop-structure-as-first-variables-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsIntTests/trivariateModels/param-with-pop-structure-as-last-variables-dim-3.txt
