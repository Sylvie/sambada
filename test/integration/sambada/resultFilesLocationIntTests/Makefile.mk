
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/resultFilesLocationIntTests/resultsInDifferentFolderAsDataFilesIntTest.cpp \
		test/integration/sambada/resultFilesLocationIntTests/resultsInDifferentFolderAsUniqueDataFileIntTest.cpp \
		test/integration/sambada/resultFilesLocationIntTests/resultsInSameFolderAsDataFilesIntTest.cpp \
		test/integration/sambada/resultFilesLocationIntTests/resultsInSameFolderAsUniqueDataFileIntTest.cpp

EXTRA_DIST += \
 		$(top_srcdir)/test/integration/sambada/resultFilesLocationIntTests/choice-env-cattle.csv \
		$(top_srcdir)/test/integration/sambada/resultFilesLocationIntTests/choice-mark-cattle.txt \
		$(top_srcdir)/test/integration/sambada/resultFilesLocationIntTests/choice-data-cattle.csv \
		$(top_srcdir)/test/integration/sambada/resultFilesLocationIntTests/expected-results-cattle-dim-0.txt \
		$(top_srcdir)/test/integration/sambada/resultFilesLocationIntTests/expected-results-cattle-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/resultFilesLocationIntTests/param-with-outputfile-setting-unique-data-file.txt \
		$(top_srcdir)/test/integration/sambada/resultFilesLocationIntTests/param-with-outputfile-setting.txt \
		$(top_srcdir)/test/integration/sambada/resultFilesLocationIntTests/param-without-outputfile-setting.txt