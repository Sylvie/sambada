include test/integration/utils/Makefile.mk

include test/integration/sambada/Makefile.mk

## Listing tests ##
check-local-test-integration: check-local-test-integration-sambada

## Cleaning ##
clean-local-test-integration: clean-local-test-integration-sambada
