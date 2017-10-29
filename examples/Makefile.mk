## Examples ##

example_files = \
	examples/data-from-manual/one-data-file/combo-data.txt \
	examples/data-from-manual/one-data-file/param-combo-a.txt \
	examples/data-from-manual/one-data-file/param-combo-b.txt \
	examples/data-from-manual/one-data-file/param-combo.txt \
	examples/data-from-manual/one-data-file/param-split-combo.txt \
	examples/data-from-manual/two-data-files/env-data.txt \
	examples/data-from-manual/two-data-files/mol-data.txt \
	examples/data-from-manual/two-data-files/param-a.txt \
	examples/data-from-manual/two-data-files/param-b.txt \
	examples/data-from-manual/two-data-files/param-split.txt \
	examples/data-from-manual/two-data-files/param.txt \
	examples/random-data/param-sample100.txt \
	examples/random-data/sample100.txt \
	examples/subset-cattle-SNP/subset-mark.txt \
	examples/subset-cattle-SNP/param-subset.txt \
	examples/subset-cattle-SNP/table-env-UG.csv

dist_pkgdata_DATA += $(example_files)

## Building archive ##
archive_examples_basename = $(archive_basename)/examples

create-archive-example-folder:
	mkdir -p $(archive_examples_basename)
	$(subst ^, , $(addprefix mkdir^-p^, $(patsubst %,^%;,$(sort $(dir $(addprefix $(archive_basename)/, $(example_files)))))))

binary-archive-local-examples: create-archive-example-folder
	$(subst ^, ,$(join $(addprefix cp^, $(addprefix $(top_srcdir)/, $(example_files))),$(patsubst %,^%;,$(dir $(addprefix $(archive_basename)/, $(example_files))))))
