
test_integration_SupervisionIntegrationTests_SOURCES += \
		test/integration/supervision/splitMarkerFileIntTests/splitMarkerFileIntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/supervision/splitMarkerFileIntTests/expected-env.txt \
		$(top_srcdir)/test/integration/supervision/splitMarkerFileIntTests/expected-mark-0-0.txt \
		$(top_srcdir)/test/integration/supervision/splitMarkerFileIntTests/expected-mark-1-12.txt \
		$(top_srcdir)/test/integration/supervision/splitMarkerFileIntTests/expected-mark-2-24.txt \
		$(top_srcdir)/test/integration/supervision/splitMarkerFileIntTests/mol-data.txt \
		$(top_srcdir)/test/integration/supervision/splitMarkerFileIntTests/param-split.txt \
		$(top_srcdir)/test/integration/supervision/splitMarkerFileIntTests/param-split-windows.txt

