## Adding unit tests for variables

test_unit_SambadaUnitTests_SOURCES +=

include test/unit/sambada/modeles/scriptorium/lecteur/Makefile.mk
include test/unit/sambada/modeles/scriptorium/scribe/Makefile.mk

## Cleaning ##
clean-local-test-unit-sambada-modeles-scriptorium: clean-local-test-unit-sambada-modeles-scriptorium-lecteur clean-local-test-unit-sambada-modeles-scriptorium-scribe
	$(call clean_extra_generated_files, test/unit/sambada/modeles/scriptorium)

