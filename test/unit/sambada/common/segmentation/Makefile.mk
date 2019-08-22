## Adding unit tests for common features

test_unit_SambadaUnitTests_SOURCES +=  \
	test/unit/sambada/common/segmentation/TokenizerTests.cpp

test_unit_SambadaUnitTests_LDADD += \
	src/common/segmentation/libsegmentation.a

## Cleaning ##
clean-local-test-unit-sambada-common-segmentation:
	$(call clean_extra_generated_files, test/unit/sambada/common/segmentation/)

