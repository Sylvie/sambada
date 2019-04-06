
test_integration_RecodePlinkIntegrationTests_SOURCES += \
		test/integration/recodePlink/recodeForLFMMIntTests/recodeAllSamplesLFMMIntTests.cpp \
		test/integration/recodePlink/recodeForLFMMIntTests/recodeSubsetSamplesLFMMIntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/recodePlink/recodeForLFMMIntTests/expected-recoded-data-lfmm-all-samples.txt \
		$(top_srcdir)/test/integration/recodePlink/recodeForLFMMIntTests/expected-recoded-data-lfmm-subset-samples.txt \
		$(top_srcdir)/test/integration/recodePlink/recodeForLFMMIntTests/selected-sample-names.txt \
		$(top_srcdir)/test/integration/recodePlink/recodeForLFMMIntTests/testPlink.map \
		$(top_srcdir)/test/integration/recodePlink/recodeForLFMMIntTests/testPlink.ped

