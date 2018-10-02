
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/resultFilesLocationIntTests/resultsInDifferentFolderAsDataIntTest.cpp \
		test/integration/sambada/resultFilesLocationIntTests/resultsInSameFolderAsDataIntTest.cpp

EXTRA_DIST += \
 		$(top_srcdir)/test/integration/sambada/resultFilesLocationIntTests/choice-env-cattle.csv \
		$(top_srcdir)/test/integration/sambada/resultFilesLocationIntTests/choice-mark-cattle.txt \
		$(top_srcdir)/test/integration/sambada/resultFilesLocationIntTests/expected-results-cattle-dim-0.txt \
		$(top_srcdir)/test/integration/sambada/resultFilesLocationIntTests/expected-results-cattle-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/resultFilesLocationIntTests/param-with-outputfile-setting.txt \
		$(top_srcdir)/test/integration/sambada/resultFilesLocationIntTests/param-without-outputfile-setting.txt