
test_integration_SupervisionIntegrationTests_SOURCES += \
		test/integration/supervision/splitMarkerFileWithoutExtensionIntTests/splitMarkerFileWithoutExtensionIntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/supervision/splitMarkerFileWithoutExtensionIntTests/expected-env.txt \
		$(top_srcdir)/test/integration/supervision/splitMarkerFileWithoutExtensionIntTests/expected-mark-0-0.txt \
		$(top_srcdir)/test/integration/supervision/splitMarkerFileWithoutExtensionIntTests/expected-mark-1-12.txt \
		$(top_srcdir)/test/integration/supervision/splitMarkerFileWithoutExtensionIntTests/expected-mark-2-24.txt \
		$(top_srcdir)/test/integration/supervision/splitMarkerFileWithoutExtensionIntTests/mol-data \
		$(top_srcdir)/test/integration/supervision/splitMarkerFileWithoutExtensionIntTests/param-split.txt \
		$(top_srcdir)/test/integration/supervision/splitMarkerFileWithoutExtensionIntTests/param-split-windows.txt

