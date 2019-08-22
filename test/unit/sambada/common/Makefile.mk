## Adding unit tests for common features

test_unit_SambadaUnitTests_SOURCES +=

test_unit_SambadaUnitTests_LDADD +=

include test/unit/sambada/common/maths/Makefile.mk
include test/unit/sambada/common/segmentation/Makefile.mk

## Cleaning ##
clean-local-test-unit-sambada-common: clean-local-test-unit-sambada-common-maths clean-local-test-unit-sambada-common-segmentation
	$(call clean_extra_generated_files, test/unit/sambada/common/)

