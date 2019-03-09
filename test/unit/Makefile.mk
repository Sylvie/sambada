include test/unit/sambada/Makefile.mk

## Listing tests ##
check-local-test-unit: check-local-test-unit-sambada

## Cleaning ##
clean-local-test-unit: clean-local-test-unit-sambada
		$(call clean_extra_generated_files, test/unit/)

