## Adding unit tests for common features

test_unit_SambadaUnitTests_SOURCES +=  \
	test/unit/sambada/common/maths/ProbabilityTests.cpp

test_unit_SambadaUnitTests_LDADD += \
	src/common/maths/libmaths.a

## Cleaning ##
clean-local-test-unit-sambada-common-maths:
	$(call clean_extra_generated_files, test/unit/sambada/common/maths/)

