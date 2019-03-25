
test_integration_SupervisionIntegrationTests_SOURCES += \
		test/integration/supervision/inputFileSplittingIntTests/inputFileSplittingIntTest.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/supervision/inputFileSplittingIntTests/expected-env.txt \
		$(top_srcdir)/test/integration/supervision/inputFileSplittingIntTests/expected-mark-0-0.txt \
		$(top_srcdir)/test/integration/supervision/inputFileSplittingIntTests/expected-mark-1-12.txt \
		$(top_srcdir)/test/integration/supervision/inputFileSplittingIntTests/expected-mark-2-24.txt \
		$(top_srcdir)/test/integration/supervision/inputFileSplittingIntTests/mol-data.txt \
		$(top_srcdir)/test/integration/supervision/inputFileSplittingIntTests/param-split.txt \
		$(top_srcdir)/test/integration/supervision/inputFileSplittingIntTests/param-split-windows.txt

