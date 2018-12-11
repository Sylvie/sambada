include test/utils/Makefile.mk

include test/integration/Makefile.mk

include test/unit/Makefile.mk

## Listing tests ##
check-local-test: check-local-test-integration check-local-test-unit

## Cleaning ##
clean-local-test: clean-local-test-integration clean-local-test-unit clean-local-test-utils
