
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/basicPopulationStructureIntTests/bivariateModels/populationStructureAsFirstVariablesBivariateModelsIntTests.cpp \
		test/integration/sambada/basicPopulationStructureIntTests/bivariateModels/populationStructureAsLastVariablesBivariateModelsIntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/bivariateModels/expected-results-bi-dim-0.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/bivariateModels/expected-results-bi-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/bivariateModels/expected-results-bi-pop-first-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/bivariateModels/expected-results-bi-pop-last-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/bivariateModels/param-bi-pop-first.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/bivariateModels/param-bi-pop-last.txt
