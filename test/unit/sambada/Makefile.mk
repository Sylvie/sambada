sambada_unit_tests = test/unit/SambadaUnitTests

noinst_PROGRAMS += ${sambada_unit_tests}
TESTS += ${sambada_unit_tests}

test_unit_SambadaUnitTests_SOURCES = ${catch_source_files} \
		test/unit/sambada/CheminAccesTests.cpp \
		test/unit/sambada/LecteurCheminAccesTests.cpp \
		src/LecteurCheminAcces.cpp \
		src/LecteurCheminAcces.hpp
test_unit_SambadaUnitTests_CPPFLAGS = -I $(top_srcdir)/src \
 		-I $(top_srcdir)/test/utils
test_unit_SambadaUnitTests_LDADD = \
		$(top_builddir)/test/utils/libSambadaTestUtils.a \
		-lstdc++fs

include test/unit/sambada/common/Makefile.mk
include test/unit/sambada/histograms/Makefile.mk
include test/unit/sambada/variables/Makefile.mk

## Listing tests ##
check-local-test-unit-sambada: ${sambada_unit_tests}
	-${sambada_unit_tests} -l
	echo "Ignored error: The return value actually counts the numbers of tests."

## Cleaning ##
clean-local-test-unit-sambada: clean-local-test-unit-sambada-variables clean-local-test-unit-sambada-histograms
	$(call clean_extra_generated_files, test/unit/sambada/)

