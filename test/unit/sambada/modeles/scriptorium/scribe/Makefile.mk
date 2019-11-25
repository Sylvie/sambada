## Adding unit tests for variables

test_unit_SambadaUnitTests_SOURCES +=  \
	test/unit/sambada/modeles/scriptorium/scribe/FlotSortieChaineFactoryTests.cpp \
	test/unit/sambada/modeles/scriptorium/scribe/ScribeTests.cpp \
	test/unit/sambada/modeles/scriptorium/scribe/ScribeModelesLineairesGeneralisesTests.cpp

## Cleaning ##
clean-local-test-unit-sambada-modeles-scriptorium-scribe:
	$(call clean_extra_generated_files, test/unit/sambada/modeles/scriptorium/scribe)

