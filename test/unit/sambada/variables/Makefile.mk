## Adding unit tests for variables

test_unit_SambadaUnitTests_SOURCES +=  \
	test/unit/sambada/variables/FamilleVariablesFactoryTests.cpp

test_unit_SambadaUnitTests_CPPFLAGS += -I $(top_srcdir)/ext/scythestat-1.0.3/scythestat \
	-I $(top_srcdir)/ext/scythestat-1.0.3/scythestat/rng

test_unit_SambadaUnitTests_CXXFLAGS = -D SCYTHE_COMPILE_DIRECT

test_unit_SambadaUnitTests_LDADD += libvariables.a

## Cleaning ##
clean-local-test-unit-sambada-variables:
	$(call clean_extra_generated_files, test/unit/sambada/variables)

