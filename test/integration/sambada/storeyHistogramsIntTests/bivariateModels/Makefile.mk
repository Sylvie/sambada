
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/storeyHistogramsIntTests/bivariateModels/storeyHistogramsWithPopStructAsFirstVarDim2IntTests.cpp \
		test/integration/sambada/storeyHistogramsIntTests/bivariateModels/storeyHistogramsWithPopuStructAsLastVarDim2IntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsIntTests/bivariateModels/expected-storey-histograms-with-pop-structure-as-first-variables-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsIntTests/bivariateModels/expected-storey-histograms-with-pop-structure-as-last-variables-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsIntTests/bivariateModels/param-with-pop-structure-as-first-variables-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/storeyHistogramsIntTests/bivariateModels/param-with-pop-structure-as-last-variables-dim-2.txt
