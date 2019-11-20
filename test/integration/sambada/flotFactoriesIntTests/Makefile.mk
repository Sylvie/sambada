
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/flotFactoriesIntTests/flotEntreeFichierFactoryIntTests.cpp \
		test/integration/sambada/flotFactoriesIntTests/flotSortieFichierFactoryIntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/flotFactoriesIntTests/input-file.txt

test_integration_SambadaIntegrationTests_LDADD += \
		$(top_builddir)/src/modeles/libmodeles.a