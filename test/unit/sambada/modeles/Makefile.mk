## Adding unit tests for variables

test_unit_SambadaUnitTests_SOURCES +=  \
	test/unit/sambada/modeles/ModeleTests.cpp

test_unit_SambadaUnitTests_LDADD += src/modeles/libmodeles.a

include test/unit/sambada/modeles/calibration/Makefile.mk
include test/unit/sambada/modeles/scriptorium/Makefile.mk

## Cleaning ##
clean-local-test-unit-sambada-modeles: clean-local-test-unit-sambada-modeles-calibration clean-local-test-unit-sambada-modeles-scriptorium
	$(call clean_extra_generated_files, test/unit/sambada/modeles)

