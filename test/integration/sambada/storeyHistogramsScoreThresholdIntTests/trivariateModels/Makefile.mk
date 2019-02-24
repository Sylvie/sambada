
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/storeyHistogramsScoreThresholdDim3IntTests.cpp \
		test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/storeyHistogramsScoreThresholdPopStructAsFirstVarsDim3IntTests.cpp \
		test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/storeyHistogramsScoreThresholdPopStructAsLastVarsDim3IntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/expected-results-no-pop-dim-0.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/expected-results-no-pop-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/expected-results-no-pop-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/expected-results-no-pop-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/expected-results-pop-first-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/expected-results-pop-first-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/expected-results-pop-last-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/expected-storey-histograms-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/expected-storey-histograms-with-pop-structure-as-first-variables-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/expected-storey-histograms-with-pop-structure-as-last-variables-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/param-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/param-with-pop-structure-as-first-variables-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/param-with-pop-structure-as-last-variables-dim-3.txt
