
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/noNewLinesEndOfFilesIntTests/noNewLinesEndOfFilesIntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/noNewLinesEndOfFilesIntTests/choice-env-cattle.csv \
		$(top_srcdir)/test/integration/sambada/noNewLinesEndOfFilesIntTests/choice-env-cattle-no-newline-eof.csv \
		$(top_srcdir)/test/integration/sambada/noNewLinesEndOfFilesIntTests/choice-mark-cattle.txt \
		$(top_srcdir)/test/integration/sambada/noNewLinesEndOfFilesIntTests/choice-mark-cattle-no-newline-eof.txt \
		$(top_srcdir)/test/integration/sambada/noNewLinesEndOfFilesIntTests/param-with-newline-eof.txt \
		$(top_srcdir)/test/integration/sambada/noNewLinesEndOfFilesIntTests/param-no-newline-eof.txt \
		$(top_srcdir)/test/integration/sambada/noNewLinesEndOfFilesIntTests/expected-results-cattle-dim-0.txt \
        $(top_srcdir)/test/integration/sambada/noNewLinesEndOfFilesIntTests/expected-results-cattle-dim-1.txt
