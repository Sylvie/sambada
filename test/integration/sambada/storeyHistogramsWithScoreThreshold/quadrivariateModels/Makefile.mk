
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/storeyHistogramsWithScoreThreshold/quadrivariateModels/storeyHistWithScoreThresholdAndPopStructAsFirstVarsDim4IntTests.cpp \
		test/integration/sambada/storeyHistogramsWithScoreThreshold/quadrivariateModels/storeyHistWithScoreThresholdAndPopStructAsLastVarsDim4IntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/quadrivariateModels/expected-results-quadri-dim-0.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/quadrivariateModels/expected-results-quadri-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/quadrivariateModels/expected-results-quadri-pop-first-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/quadrivariateModels/expected-results-quadri-pop-first-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/quadrivariateModels/expected-results-quadri-pop-first-dim-4.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/quadrivariateModels/expected-results-quadri-pop-last-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/quadrivariateModels/expected-results-quadri-pop-last-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/quadrivariateModels/expected-results-quadri-pop-last-dim-4.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/quadrivariateModels/expected-storey-histograms-with-pop-structure-as-first-variables-dim-4.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/quadrivariateModels/expected-storey-histograms-with-pop-structure-as-last-variables-dim-4.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/quadrivariateModels/param-with-pop-structure-as-first-variables-dim-4.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsWithScoreThreshold/quadrivariateModels/param-with-pop-structure-as-last-variables-dim-4.txt
