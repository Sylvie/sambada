## Adding unit tests for variables

test_unit_SambadaUnitTests_SOURCES +=  \
	test/unit/sambada/variables/FamilleVariablesFactoryTests.cpp \
	src/variables/CombinaisonVariables.cpp \
    src/variables/CombinaisonVariables.hpp \
    src/variables/FamilleVariablesFactory.cpp \
    src/variables/FamilleVariablesFactory.hpp \
    src/variables/TypesCommuns.hpp

test_unit_SambadaUnitTests_CPPFLAGS += -I $(top_srcdir)/ext/scythestat-1.0.3/scythestat \
	-I $(top_srcdir)/ext/scythestat-1.0.3/scythestat/rng
test_unit_SambadaUnitTests_CXXFLAGS = -D SCYTHE_COMPILE_DIRECT

## Cleaning ##
clean-local-test-unit-sambada-variables:
	$(call clean_extra_generated_files, test/unit/sambada/variables)

