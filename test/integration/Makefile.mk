
include test/integration/utils/Makefile.mk

include test/integration/sambada/Makefile.mk

## Cleaning ##
clean-local-test-integration: clean-local-test-integration-sambada
