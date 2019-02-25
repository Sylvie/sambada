
test_integration_SambadaIntegrationTests_SOURCES +=

EXTRA_DIST += \
 		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/cattle-pop-env-first.csv \
 		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/cattle-pop-env-last.csv \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/cattle-pop-mark.txt
		
include test/integration/sambada/storeyHistogramsScoreThresholdIntTests/univariateModels/Makefile.mk
include test/integration/sambada/storeyHistogramsScoreThresholdIntTests/bivariateModels/Makefile.mk
include test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/Makefile.mk
include test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/Makefile.mk
include test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/Makefile.mk
include test/integration/sambada/storeyHistogramsScoreThresholdIntTests/quadrivariateModels/Makefile.mk
