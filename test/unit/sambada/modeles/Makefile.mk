## Adding unit tests for variables

test_unit_SambadaUnitTests_SOURCES +=  \
	test/unit/sambada/modeles/ModeleTests.cpp

test_unit_SambadaUnitTests_LDADD += src/modeles/libmodeles.a

## Cleaning ##
clean-local-test-unit-sambada-modeles:
	$(call clean_extra_generated_files, test/unit/sambada/modeles)

