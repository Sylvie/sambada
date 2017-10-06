## Examples ##

example_files = \
	examples/DataFromManual/OneDataFile/combo-data.txt \
	examples/DataFromManual/OneDataFile/param-combo-a.txt \
	examples/DataFromManual/OneDataFile/param-combo-b.txt \
	examples/DataFromManual/OneDataFile/param-combo.txt \
	examples/DataFromManual/OneDataFile/param-split-combo.txt \
	examples/DataFromManual/TwoDataFiles/env-data.txt \
	examples/DataFromManual/TwoDataFiles/mol-data.txt \
	examples/DataFromManual/TwoDataFiles/param-a.txt \
	examples/DataFromManual/TwoDataFiles/param-b.txt \
	examples/DataFromManual/TwoDataFiles/param-split.txt \
	examples/DataFromManual/TwoDataFiles/param.txt \
	examples/RandomData/input-Sample100.txt \
	examples/RandomData/Sample100.txt \
	examples/SubsetCattleSNP/extrait_marq.txt \
	examples/SubsetCattleSNP/paramTest.txt \
	examples/SubsetCattleSNP/TableEnvUG.csv

dist_pkgdata_DATA += $(example_files)

## Building archive ##
archive_examples_basename = $(archive_basename)/examples

create-archive-example-folder:
	mkdir -p $(archive_examples_basename)

binary-archive-local-examples: create-archive-example-folder
	cp $(example_files) $(archive_examples_basename)
