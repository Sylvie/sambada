## Adding unit tests for variables

test_unit_SambadaUnitTests_SOURCES +=  \
	test/unit/sambada/parametres/LecteurParametresTests.cpp

test_unit_SambadaUnitTests_LDADD += src/parametres/libparametres.a

## Cleaning ##
clean-local-test-unit-sambada-parametres:
	$(call clean_extra_generated_files, test/unit/sambada/parametres)

