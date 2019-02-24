
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/storeyHistogramsIntTests/quadrivariateModels/storeyHistogramsWithPopStructAsFirstVarsDim4IntTests.cpp \
		test/integration/sambada/storeyHistogramsIntTests/quadrivariateModels/storeyHistogramsWithPopStructAsLastVarsDim4IntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsIntTests/quadrivariateModels/expected-storey-histograms-with-pop-structure-as-first-variables-dim-4.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsIntTests/quadrivariateModels/expected-storey-histograms-with-pop-structure-as-last-variables-dim-4.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsIntTests/quadrivariateModels/param-with-pop-structure-as-first-variables-dim-4.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsIntTests/quadrivariateModels/param-with-pop-structure-as-last-variables-dim-4.txt
