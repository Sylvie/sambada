## Adding unit tests for variables

test_unit_SambadaUnitTests_SOURCES +=  \
	test/unit/sambada/histograms/HistogramTests.cpp \
	test/unit/sambada/histograms/GroupHistogramsTests.cpp \
	test/unit/sambada/histograms/StoreyHistogramsTests.cpp \
	test/unit/sambada/histograms/StoreyHistogramsStreamWriterTests.cpp

test_unit_SambadaUnitTests_LDADD += src/histograms/libhistograms.a

## Cleaning ##
clean-local-test-unit-sambada-histograms:
	$(call clean_extra_generated_files, test/unit/sambada/histograms)

