## Adding unit tests for variables

test_unit_SambadaUnitTests_SOURCES +=  \
	test/unit/sambada/modeles/scriptorium/lecteur/FlotEntreeChaineFactoryTests.cpp

## Cleaning ##
clean-local-test-unit-sambada-modeles-scriptorium-lecteur:
	$(call clean_extra_generated_files, test/unit/sambada/modeles/scriptorium/lecteur)

