##########################################################################
# Copyright (©) 2011-2020 EPFL (Ecole Polytechnique fédérale de Lausanne)
# Laboratory of Geographic information systems (LaSIG)
#
# This file is part of Sambada.
#
# Sambada is free software ; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation ; either version 3 of the License, or (at your option) any later version.
# Sambada is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY ; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
# You should have received a copy of the GNU General Public License along with Sambada ; if not, see <http://www.gnu.org/licenses/>.
#
# Authors : Sylvie Stucki (sylvie.stucki@a3.epfl.ch), Stéphane Joost (stephane.joost@epfl.ch)
# Laboratory of Geographic information systems
# EPFL ENAC IIE LASIG
# Station 18
# CH-1015 Lausanne
# Web site : http://lasig.epfl.ch/sambada
#
# Sambada includes two libraries: Scythe Statistical Library (under GPL 3) and Shapefile C Library (under LGPL 2.1, courtesy of Frank Warmerdam).
#
# Scythe Statistical Library
# Copyright (C) 2000-2002 Andrew D. Martin and Kevin M. Quinn;
# 2002-2012 Andrew D. Martin, Kevin M. Quinn, and Daniel Pemstein.  All Rights Reserved.
#
# Shapefile C Library
# Copyright (c) 1999, Frank Warmerdam
##########################################################################

## Examples ##

example_files = \
	examples/analysis-with-population-structure/cattle-pop-env-first.csv \
	examples/analysis-with-population-structure/cattle-pop-env-last.csv \
	examples/analysis-with-population-structure/cattle-pop-mark.txt \
	examples/analysis-with-population-structure/param-pop-first.txt \
	examples/analysis-with-population-structure/param-pop-last.txt \
	examples/analysis-with-population-structure/ReadMe.txt \
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
	examples/random-data/param-random-sample.txt \
	examples/random-data/random-sample.txt \
	examples/subset-cattle-SNP/cattle-env.csv \
	examples/subset-cattle-SNP/cattle-mark.txt \
	examples/subset-cattle-SNP/param-cattle.txt

dist_pkgdata_DATA += $(example_files)

## Building archive ##
archive_examples_basename = $(archive_basename)/examples

create-archive-example-folder:
	mkdir -p $(archive_examples_basename)
	$(subst ^, , $(addprefix mkdir^-p^, $(patsubst %,^%;,$(sort $(dir $(addprefix $(archive_basename)/, $(example_files)))))))

binary-archive-local-examples: create-archive-example-folder
	$(subst ^, ,$(join $(addprefix cp^, $(addprefix $(top_srcdir)/, $(example_files))),$(patsubst %,^%;,$(dir $(addprefix $(archive_basename)/, $(example_files))))))
