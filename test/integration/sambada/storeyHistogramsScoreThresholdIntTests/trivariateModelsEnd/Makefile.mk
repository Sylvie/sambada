
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/storeyHistogramsScoreThresholdDim3IntTests.cpp \
		test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/storeyHistogramsScoreThresholdPopStructAsFirstVarsDim3IntTests.cpp \
		test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/storeyHistogramsScoreThresholdPopStructAsLastVarsDim3IntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/expected-results-no-pop-dim-0.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/expected-results-no-pop-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/expected-results-no-pop-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/expected-results-no-pop-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/expected-results-pop-first-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/expected-results-pop-first-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/expected-results-pop-last-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/expected-storey-histograms-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/expected-storey-histograms-with-pop-structure-as-first-variables-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/expected-storey-histograms-with-pop-structure-as-last-variables-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/param-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/param-with-pop-structure-as-first-variables-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/param-with-pop-structure-as-last-variables-dim-3.txt
