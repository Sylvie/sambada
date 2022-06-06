
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/wrongNumberOfIndividualsIntTests/wrongNumberOfIndividualsInEnvironmentalDataIntTests.cpp \
		test/integration/sambada/wrongNumberOfIndividualsIntTests/wrongNumberOfIndividualsInMolecularDataIntTests.cpp

EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/wrongNumberOfIndividualsIntTests/choice-env-cattle.csv \
		$(top_srcdir)/test/integration/sambada/wrongNumberOfIndividualsIntTests/choice-env-cattle-too-many-individuals.csv \
		$(top_srcdir)/test/integration/sambada/wrongNumberOfIndividualsIntTests/choice-env-cattle-too-few-individuals.csv \
		$(top_srcdir)/test/integration/sambada/wrongNumberOfIndividualsIntTests/choice-env-cattle-too-many-individuals-no-newline-eof.csv \
		$(top_srcdir)/test/integration/sambada/wrongNumberOfIndividualsIntTests/choice-env-cattle-too-few-individuals-no-newline-eof.csv \
		$(top_srcdir)/test/integration/sambada/wrongNumberOfIndividualsIntTests/choice-mark-cattle.txt \
		$(top_srcdir)/test/integration/sambada/wrongNumberOfIndividualsIntTests/choice-mark-cattle-too-many-individuals.txt \
		$(top_srcdir)/test/integration/sambada/wrongNumberOfIndividualsIntTests/choice-mark-cattle-too-few-individuals.txt \
		$(top_srcdir)/test/integration/sambada/wrongNumberOfIndividualsIntTests/choice-mark-cattle-too-many-individuals-no-newline-eof.txt \
		$(top_srcdir)/test/integration/sambada/wrongNumberOfIndividualsIntTests/choice-mark-cattle-too-few-individuals-no-newline-eof.txt \
		$(top_srcdir)/test/integration/sambada/wrongNumberOfIndividualsIntTests/param-wrong-number-individuals.txt
