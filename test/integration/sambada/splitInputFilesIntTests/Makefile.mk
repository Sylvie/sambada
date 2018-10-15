
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/splitInputFilesIntTests/splitInputFilesWithFilenamesExtensionsIntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/splitInputFilesIntTests/choice-env-cattle.csv \
		$(top_srcdir)/test/integration/sambada/splitInputFilesIntTests/choice-mark-cattle.txt \
		$(top_srcdir)/test/integration/sambada/splitInputFilesIntTests/choice-mark-cattle-mark-0-0.txt \
		$(top_srcdir)/test/integration/sambada/splitInputFilesIntTests/choice-mark-cattle-mark-1-3.txt \
		$(top_srcdir)/test/integration/sambada/splitInputFilesIntTests/param-0-0-no-outputfile.txt \
		$(top_srcdir)/test/integration/sambada/splitInputFilesIntTests/param-0-0-outputfile-no-ext.txt \
		$(top_srcdir)/test/integration/sambada/splitInputFilesIntTests/param-0-0-outputfile-with-ext.txt \
		$(top_srcdir)/test/integration/sambada/splitInputFilesIntTests/param-1-3-no-outputfile.txt \
		$(top_srcdir)/test/integration/sambada/splitInputFilesIntTests/param-1-3-outputfile-no-ext.txt \
		$(top_srcdir)/test/integration/sambada/splitInputFilesIntTests/param-1-3-outputfile-with-ext.txt \
		$(top_srcdir)/test/integration/sambada/splitInputFilesIntTests/param-split.txt \
		$(top_srcdir)/test/integration/sambada/splitInputFilesIntTests/expected-results-cattle-dim-0.txt \
 		$(top_srcdir)/test/integration/sambada/splitInputFilesIntTests/expected-results-cattle-dim-0-0-0.txt \
		$(top_srcdir)/test/integration/sambada/splitInputFilesIntTests/expected-results-cattle-dim-0-1-3.txt \
		$(top_srcdir)/test/integration/sambada/splitInputFilesIntTests/expected-results-cattle-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/splitInputFilesIntTests/expected-results-cattle-dim-1-0-0.txt \
      	$(top_srcdir)/test/integration/sambada/splitInputFilesIntTests/expected-results-cattle-dim-1-1-3.txt
