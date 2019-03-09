
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/basicPopulationStructureIntTests/populationStructureWithSavetypeBestIntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/cattle-pop-env-first.csv \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/cattle-pop-env-last.csv \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/cattle-pop-mark.txt \
		$(top_srcdir)/test/integration/sambada/basicPopulationStructureIntTests/param-pop-first-savetype-best.txt

include test/integration/sambada/basicPopulationStructureIntTests/bivariateModels/Makefile.mk
include test/integration/sambada/basicPopulationStructureIntTests/trivariateModels/Makefile.mk
include test/integration/sambada/basicPopulationStructureIntTests/trivariateModelsReal/Makefile.mk
include test/integration/sambada/basicPopulationStructureIntTests/quadrivariateModels/Makefile.mk
