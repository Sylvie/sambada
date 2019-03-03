
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/storeyHistogramsIntTests/storeyHistogramsWithSavetypeBestIntTests.cpp

EXTRA_DIST += \
 		$(top_srcdir)/test/integration/sambada/storeyHistogramsIntTests/cattle-pop-env-first.csv \
 		$(top_srcdir)/test/integration/sambada/storeyHistogramsIntTests/cattle-pop-env-last.csv \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsIntTests/cattle-pop-mark.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsIntTests/param-savetype-best.txt

include test/integration/sambada/storeyHistogramsIntTests/univariateModels/Makefile.mk
include test/integration/sambada/storeyHistogramsIntTests/bivariateModels/Makefile.mk
include test/integration/sambada/storeyHistogramsIntTests/trivariateModels/Makefile.mk
include test/integration/sambada/storeyHistogramsIntTests/trivariateModelsReal/Makefile.mk
include test/integration/sambada/storeyHistogramsIntTests/quadrivariateModels/Makefile.mk
