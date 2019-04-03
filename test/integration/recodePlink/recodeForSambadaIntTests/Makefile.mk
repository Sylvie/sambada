
test_integration_RecodePlinkIntegrationTests_SOURCES += \
		test/integration/recodePlink/recodeForSambadaIntTests/recodeAllSamplesIntTests.cpp \
		test/integration/recodePlink/recodeForSambadaIntTests/recodeSubsetSamplesIntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/recodePlink/recodeForSambadaIntTests/expected-recoded-data-all-samples.txt \
		$(top_srcdir)/test/integration/recodePlink/recodeForSambadaIntTests/expected-recoded-data-subset-samples.txt \
		$(top_srcdir)/test/integration/recodePlink/recodeForSambadaIntTests/selected-sample-names.txt \
		$(top_srcdir)/test/integration/recodePlink/recodeForSambadaIntTests/testPlink.map \
		$(top_srcdir)/test/integration/recodePlink/recodeForSambadaIntTests/testPlink.ped

