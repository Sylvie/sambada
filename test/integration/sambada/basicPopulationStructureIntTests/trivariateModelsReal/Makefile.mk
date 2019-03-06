
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/basicPopulationStructureIntTests/trivariateModelsReal/populationStructureAsNormalVariablesIntTests.cpp \
		test/integration/sambada/basicPopulationStructureIntTests/trivariateModelsReal/populationStructureAsFirstVariablesIntTests.cpp \
		test/integration/sambada/basicPopulationStructureIntTests/trivariateModelsReal/populationStructureAsLastVariablesIntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/trivariateModelsReal/expected-results-no-pop-dim-0.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/trivariateModelsReal/expected-results-no-pop-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/trivariateModelsReal/expected-results-no-pop-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/trivariateModelsReal/expected-results-no-pop-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/trivariateModelsReal/expected-results-pop-first-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/trivariateModelsReal/expected-results-pop-first-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/trivariateModelsReal/expected-results-pop-last-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/trivariateModelsReal/param-no-pop.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/trivariateModelsReal/param-pop-first.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/trivariateModelsReal/param-pop-last.txt
