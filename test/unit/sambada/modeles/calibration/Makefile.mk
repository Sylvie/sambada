## Adding unit tests for variables

test_unit_SambadaUnitTests_SOURCES += \
	test/unit/sambada/modeles/calibration/PreparateurRegressionLogistiqueTests.cpp \
	test/unit/sambada/modeles/calibration/RegresseurLogistiqueTests.cpp

## Cleaning ##
clean-local-test-unit-sambada-modeles-calibration:
	$(call clean_extra_generated_files, test/unit/sambada/modeles/calibration)

