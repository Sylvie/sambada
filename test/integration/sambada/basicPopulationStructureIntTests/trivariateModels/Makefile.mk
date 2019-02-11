
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/basicPopulationStructureIntTests/trivariateModels/populationStructureAsNormalVariablesIntTests.cpp \
		test/integration/sambada/basicPopulationStructureIntTests/trivariateModels/populationStructureAsFirstVariablesIntTests.cpp \
		test/integration/sambada/basicPopulationStructureIntTests/trivariateModels/populationStructureAsLastVariablesIntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/trivariateModels/expected-results-no-pop-dim-0.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/trivariateModels/expected-results-no-pop-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/trivariateModels/expected-results-no-pop-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/trivariateModels/expected-results-no-pop-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/trivariateModels/expected-results-pop-first-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/trivariateModels/expected-results-pop-first-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/trivariateModels/expected-results-pop-last-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/trivariateModels/param-no-pop.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/trivariateModels/param-pop-first.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/trivariateModels/param-pop-last.txt
