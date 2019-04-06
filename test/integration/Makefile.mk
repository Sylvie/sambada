include test/integration/utils/Makefile.mk

include test/integration/sambada/Makefile.mk

include test/integration/supervision/Makefile.mk

include test/integration/recodePlink/Makefile.mk

## Listing tests ##
check-local-test-integration: check-local-test-integration-sambada check-local-test-integration-supervision check-local-test-integration-recode-plink

## Cleaning ##
clean-local-test-integration: clean-local-test-integration-sambada clean-local-test-integration-utils clean-local-test-integration-supervision clean-local-test-integration-recode-plink
	$(call clean_extra_generated_files, test/integration/)
