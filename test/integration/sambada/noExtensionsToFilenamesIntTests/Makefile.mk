
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/noExtensionsToFilenamesIntTests/noExtensionsToFilenamesIntTests.cpp \
		test/integration/sambada/noExtensionsToFilenamesIntTests/noExtensionsToFilenamesUniqueFileIntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/noExtensionsToFilenamesIntTests/choice-env-cattle.csv \
		$(top_srcdir)/test/integration/sambada/noExtensionsToFilenamesIntTests/choice-env-cattle \
		$(top_srcdir)/test/integration/sambada/noExtensionsToFilenamesIntTests/choice-mark-cattle.txt \
		$(top_srcdir)/test/integration/sambada/noExtensionsToFilenamesIntTests/choice-mark-cattle \
		$(top_srcdir)/test/integration/sambada/noExtensionsToFilenamesIntTests/param.txt \
		$(top_srcdir)/test/integration/sambada/noExtensionsToFilenamesIntTests/param \
		$(top_srcdir)/test/integration/sambada/noExtensionsToFilenamesIntTests/param-marker-file-no-ext.txt \
		$(top_srcdir)/test/integration/sambada/noExtensionsToFilenamesIntTests/expected-results-cattle-dim-0.txt \
        $(top_srcdir)/test/integration/sambada/noExtensionsToFilenamesIntTests/expected-results-cattle-dim-1.txt
