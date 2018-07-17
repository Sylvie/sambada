sambada_integration_tests =
# test/integration/HelloIntegrationTests \
 #		test/integration/WriteModelsTests \
# 		test/integration/CompareModelsTests

noinst_PROGRAMS += ${sambada_integration_tests}
TESTS += ${sambada_integration_tests}

#test_integration_HelloIntegrationTests_SOURCES = test/integration/HelloIntegrationTests.cpp ${catch_source_files}
#test_integration_HelloIntegrationTests_CPPFLAGS = -I $(top_srcdir)/src \
# 		-I $(top_srcdir)/test/utils \
# 		-I $(top_srcdir)/test/integration/utils \
# 		-Dsambada_executable_extension=\"${sambada_executable_extension}\"
#test_integration_HelloIntegrationTests_LDADD = $(top_builddir)/test/integration/utils/libSambadaIntegrationTestUtils.a
#
#test_integration_WriteModelsTests_SOURCES = test/integration/WriteModelsTests.cpp ${catch_source_files}
#test_integration_WriteModelsTests_CPPFLAGS = -I $(top_srcdir)/test/utils -I $(top_srcdir)/test/integration/utils -Dsambada_executable_extension=\"${sambada_executable_extension}\"
#test_integration_WriteModelsTests_LDADD = $(top_builddir)/test/integration/utils/libSambadaIntegrationTestUtils.a
#
#test_integration_CompareModelsTests_SOURCES = test/integration/CompareModelsTests.cpp ${catch_source_files}
#test_integration_CompareModelsTests_CPPFLAGS = -I $(top_srcdir)/src \
# 		-I $(top_srcdir)/test/utils \
# 		-I $(top_srcdir)/test/integration/utils \
# 		-Dsambada_executable_extension=\"${sambada_executable_extension}\"
#
#test_integration_CompareModelsTests_LDADD = \
# 		$(top_builddir)/test/integration/utils/libSambadaIntegrationTestUtils.a \
# 		$(top_builddir)/test/utils/libSambadaTestUtils.a

#EXTRA_DIST += $(top_srcdir)/test/integration/expected-cattle-mark-Out-1.txt

include test/integration/utils/Makefile.mk
