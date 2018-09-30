include test/utils/Makefile.mk

include test/integration/Makefile.mk

## Listing tests ##
check-local-test: check-local-test-integration

## Cleaning ##
clean-local-test: clean-local-test-integration
