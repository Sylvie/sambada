
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/basicPopulationStructureIntTests/populationStructureAsNormalVariablesIntTests.cpp

EXTRA_DIST += \
 		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/cattle-pop-env.csv \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/cattle-pop-mark.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/expected-results-no-pop-dim-0.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/expected-results-no-pop-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/expected-results-no-pop-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/expected-results-no-pop-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/param-no-pop.txt
