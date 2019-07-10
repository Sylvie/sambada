catch_source_files = \
	test/utils/main-catch.cpp \
	test/utils/catch.hpp

noinst_LIBRARIES += test/utils/libSambadaTestUtils.a

test_utils_libSambadaTestUtils_a_SOURCES = 	\
    test/utils/SambadaRegressionResults.cpp \
    test/utils/SambadaRegressionResults.hpp \
    test/utils/SambadaSpatialAutocorrelationResults.cpp \
    test/utils/SambadaSpatialAutocorrelationResults.hpp	\
    test/utils/SambadaStoreyHistogram.cpp \
    test/utils/SambadaStoreyHistogram.hpp \
    test/utils/SambadaInputData.cpp \
    test/utils/SambadaInputData.hpp \
    test/utils/SambadaStoreyHistogramMapper.hpp \
    test/utils/SambadaStoreyHistogramMapper.cpp \
    test/utils/catch.hpp
test_utils_libSambadaTestUtils_a_CPPFLAGS = -I $(top_srcdir)/test/utils/

## Cleaning ##
clean-local-test-utils:
	$(call clean_extra_generated_files, test/utils/)
