include test/integration/utils/Makefile.mk

include test/integration/sambada/Makefile.mk

include test/integration/supervision/Makefile.mk

## Listing tests ##
check-local-test-integration: check-local-test-integration-sambada check-local-test-integration-supervision

## Cleaning ##
clean-local-test-integration: clean-local-test-integration-sambada clean-local-test-integration-utils clean-local-test-integration-supervision
	$(call clean_extra_generated_files, test/integration/)
