
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/storeyHistograms/storeyHistogramsWithSavetypeBestIntTests.cpp

EXTRA_DIST += \
 		$(top_srcdir)/test/integration/sambada/storeyHistograms/cattle-pop-env-first.csv \
 		$(top_srcdir)/test/integration/sambada/storeyHistograms/cattle-pop-env-last.csv \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/cattle-pop-mark.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistograms/param-savetype-best.txt

include test/integration/sambada/storeyHistograms/univariateModels/Makefile.mk
include test/integration/sambada/storeyHistograms/bivariateModels/Makefile.mk
include test/integration/sambada/storeyHistograms/trivariateModels/Makefile.mk
include test/integration/sambada/storeyHistograms/quadrivariateModels/Makefile.mk
