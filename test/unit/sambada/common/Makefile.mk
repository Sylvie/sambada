## Adding unit tests for common features

test_unit_SambadaUnitTests_SOURCES +=  \
	test/unit/sambada/common/ProbabilityTests.cpp

test_unit_SambadaUnitTests_LDADD += src/common/libcommon.a

## Cleaning ##
clean-local-test-unit-sambada-histograms:
	$(call clean_extra_generated_files, test/unit/sambada/histograms)

