
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/storeyHistogramsScoreThresholdIntTests/bivariateModels/storeyHistogramsScoreThresholdPopStructAsFirstVarDim2IntTests.cpp \
		test/integration/sambada/storeyHistogramsScoreThresholdIntTests/bivariateModels/storeyHistogramsScoreThresholdPopStructAsLastVarDim2IntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/bivariateModels/expected-results-bi-dim-0.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/bivariateModels/expected-results-bi-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/bivariateModels/expected-results-bi-pop-first-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/bivariateModels/expected-results-bi-pop-last-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/bivariateModels/expected-storey-histograms-with-pop-structure-as-first-variables-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/bivariateModels/expected-storey-histograms-with-pop-structure-as-last-variables-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/bivariateModels/param-with-pop-structure-as-first-variables-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/bivariateModels/param-with-pop-structure-as-last-variables-dim-2.txt
