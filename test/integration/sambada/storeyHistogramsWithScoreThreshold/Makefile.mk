
test_integration_SambadaIntegrationTests_SOURCES +=

EXTRA_DIST += \
 		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/cattle-pop-env-first.csv \
 		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/cattle-pop-env-last.csv \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/cattle-pop-mark.txt
		
include test/integration/sambada/storeyHistogramsWithScoreThreshold/univariateModels/Makefile.mk
include test/integration/sambada/storeyHistogramsWithScoreThreshold/bivariateModels/Makefile.mk
include test/integration/sambada/storeyHistogramsWithScoreThreshold/trivariateModels/Makefile.mk
include test/integration/sambada/storeyHistogramsWithScoreThreshold/quadrivariateModels/Makefile.mk
