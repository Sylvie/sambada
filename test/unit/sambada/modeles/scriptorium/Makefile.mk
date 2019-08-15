## Adding unit tests for variables

test_unit_SambadaUnitTests_SOURCES +=  \
	test/unit/sambada/modeles/scriptorium/FlotSortieChaineFactoryTests.cpp \
	test/unit/sambada/modeles/scriptorium/ScribeTests.cpp

include test/unit/sambada/modeles/scriptorium/lecteur/Makefile.mk

## Cleaning ##
clean-local-test-unit-sambada-modeles-scriptorium: clean-local-test-unit-sambada-modeles-scriptorium-lecteur
	$(call clean_extra_generated_files, test/unit/sambada/modeles/scriptorium)

