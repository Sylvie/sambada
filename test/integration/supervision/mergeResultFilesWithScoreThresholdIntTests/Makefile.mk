
test_integration_SupervisionIntegrationTests_SOURCES += \
		test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/mergeResultFilesWithGScoreThresholdIntTests.cpp \
		test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/mergeResultFilesWithWaldScoreThresholdIntTests.cpp \
		test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/mergeResultFilesWithBothScoreThresholdIntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/expected-results-dim-0.txt \
		$(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/expected-results-G-dim-1.txt \
		$(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/expected-results-G-dim-2.txt \
		$(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/expected-results-G-dim-3.txt \
		$(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/expected-results-Wald-dim-1.txt \
		$(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/expected-results-Wald-dim-2.txt \
		$(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/expected-results-Wald-dim-3.txt \
		$(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/expected-results-both-dim-1.txt \
        $(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/expected-results-both-dim-2.txt \
        $(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/expected-results-both-dim-3.txt \
        $(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/mol-data-mark-0-0-Out-0.txt \
		$(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/mol-data-mark-0-0-Out-1.txt \
		$(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/mol-data-mark-0-0-Out-2.txt \
		$(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/mol-data-mark-0-0-Out-3.txt \
		$(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/mol-data-mark-1-12-Out-0.txt \
		$(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/mol-data-mark-1-12-Out-1.txt \
		$(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/mol-data-mark-1-12-Out-2.txt \
		$(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/mol-data-mark-1-12-Out-3.txt \
		$(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/mol-data-mark-2-24-Out-0.txt \
		$(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/mol-data-mark-2-24-Out-1.txt \
		$(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/mol-data-mark-2-24-Out-2.txt \
		$(top_srcdir)/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/mol-data-mark-2-24-Out-3.txt
