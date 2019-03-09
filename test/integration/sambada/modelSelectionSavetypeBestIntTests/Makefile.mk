
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/modelSelectionSavetypeBestIntTests/modelSelectionSavetypeAllIntTests.cpp \
		test/integration/sambada/modelSelectionSavetypeBestIntTests/modelSelectionSavetypeBestIntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/modelSelectionSavetypeBestIntTests/cattle-pop-env.csv \
		$(top_srcdir)/test/integration/sambada/modelSelectionSavetypeBestIntTests/cattle-pop-mark.txt \
		$(top_srcdir)/test/integration/sambada/modelSelectionSavetypeBestIntTests/expected-results-savetype-all-dim-0.txt \
		$(top_srcdir)/test/integration/sambada/modelSelectionSavetypeBestIntTests/expected-results-savetype-all-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/modelSelectionSavetypeBestIntTests/expected-results-savetype-all-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/modelSelectionSavetypeBestIntTests/expected-results-savetype-all-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/modelSelectionSavetypeBestIntTests/expected-results-savetype-best-dim-0.txt \
		$(top_srcdir)/test/integration/sambada/modelSelectionSavetypeBestIntTests/expected-results-savetype-best-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/modelSelectionSavetypeBestIntTests/expected-results-savetype-best-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/modelSelectionSavetypeBestIntTests/expected-results-savetype-best-dim-3.txt \
		$(top_srcdir)/test/integration/sambada/modelSelectionSavetypeBestIntTests/param-savetype-all.txt \
		$(top_srcdir)/test/integration/sambada/modelSelectionSavetypeBestIntTests/param-savetype-best.txt
