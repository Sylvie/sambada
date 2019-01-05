catch_source_files = \
	test/utils/main-catch.cpp \
	test/utils/catch.hpp

noinst_LIBRARIES += test/utils/libSambadaTestUtils.a

test_utils_libSambadaTestUtils_a_SOURCES = 	\
    test/utils/SambadaRegressionResults.cpp \
    test/utils/SambadaRegressionResults.h 	\
    test/utils/SambadaSpatialAutocorrelationResults.cpp \
    test/utils/SambadaSpatialAutocorrelationResults.h 	\
    test/utils/catch.hpp
test_utils_libSambadaTestUtils_a_CPPFLAGS = -I $(top_srcdir)/test/utils/

## Cleaning ##
clean-local-test-utils:
	$(call clean_extra_generated_files, test/utils/)
