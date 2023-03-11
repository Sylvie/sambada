##########################################################################
# Copyright (©) 2011-2023 EPFL (Ecole Polytechnique fédérale de Lausanne)
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

manual_filename = manual.tex
manual_directory := manual
manual_sources := $(top_srcdir)/$(manual_directory)/$(manual_filename) $(top_srcdir)/$(manual_directory)/manual.bib
manual_output_filename := sambadoc
manual_pdf := $(builddir)/$(manual_directory)/$(manual_output_filename).pdf
manual_txt := $(builddir)/$(manual_directory)/manual.txt

EXTRA_DIST += $(manual_sources) $(top_srcdir)/$(manual_directory)/latexmkrc_template $(builddir)/$(manual_directory)/latexmkrc
BUILT_SOURCES += $(manual_directory)/latexmkrc

if HAVE_LATEXMK
EXTRA_DIST += $(manual_pdf)
endif

LATEXMK_BUILD = $(LATEXMK) -r $(builddir)/$(manual_directory)/latexmkrc -output-directory=$(manual_directory) -jobname=$(manual_output_filename)
LATEXMK_CLEAN = $(LATEXMK) -r ../$(builddir)/$(manual_directory)/latexmkrc -jobname=$(manual_output_filename)

prepare_manual_directory:
	if [ "$(top_srcdir)" != "$(top_builddir)" ]; then \
		mkdir -p $(manual_directory) && \
		if test $(top_srcdir)/$(manual_directory)/manual.bib -nt $(manual_directory)/manual.bib; then \
			cp -f $(top_srcdir)/$(manual_directory)/manual.bib $(manual_directory); \
		fi && \
		if test $(top_srcdir)/$(manual_directory)/manual.tex -nt $(manual_directory)/manual.tex; then \
			cp -f $(top_srcdir)/$(manual_directory)/manual.tex $(manual_directory); \
		fi \
	fi

prepare_latexmkrc_file: | prepare_manual_directory
	sed "s/%versionNumber%/$(VERSION)/g; s/%releaseDate%/$(sambada_releasedate)/g" $(top_srcdir)/$(manual_directory)/latexmkrc_template > $(builddir)/$(manual_directory)/latexmkrc ;

$(manual_directory)/latexmkrc: prepare_latexmkrc_file

if HAVE_LATEXMK
pdf-local-manual: $(manual_pdf) $(manual_directory)/latexmkrc

$(manual_directory)/$(manual_output_filename).pdf : $(top_srcdir)/$(manual_directory)/$(manual_filename) $(manual_directory)/latexmkrc
	$(LATEXMK_BUILD) -pdf $<

%.pdf : %.tex $(manual_directory)/latexmkrc
	$(LATEXMK_BUILD) -pdf $<

binary-archive-local-manual:
	cp -f $(manual_pdf) $(archive_basename)

clean-local-manual: prepare_latexmkrc_file
	if [ -d "$(manual_directory)" ] && [ -e "$(manual_directory)/$(manual_filename)" ] ; then \
		cd $(manual_directory) && \
		pwd && \
		$(LATEXMK_CLEAN) -c $(manual_filename) ; \
	fi

distclean-local-manual:
	if [ -d "$(manual_directory)" ] && [ -e "$(manual_directory)/$(manual_filename)" ]; then \
		cd $(manual_directory) && \
		$(LATEXMK_CLEAN) -C $(manual_filename) ; \
	fi
	if [ -d "$(manual_directory)" ] && [ "$(top_srcdir)" != "$(top_builddir)" ]; then \
		rm -f $(manual_directory)/manual.bib ; \
		rm -f $(manual_directory)/manual.tex ; \
	fi
	if [ -d "$(manual_directory)" ]; then \
		rm -f $(manual_directory)/latexmkrc ; \
	fi

else
pdf-local-manual: | prepare_manual_directory
	sed "s/%versionNumber%/$(VERSION)/g" $(top_srcdir)/$(manual_directory)/no_manual_template > $(manual_txt) ;

binary-archive-local-manual:
	cp -f $(manual_txt) $(archive_basename)

clean-local-manual:
	rm -f $(manual_txt)

distclean-local-manual:
	if [ -d "$(manual_directory)" ] && [ "$(top_srcdir)" != "$(top_builddir)" ]; then \
		rm -f $(manual_directory)/manual.bib ; \
		rm -f $(manual_directory)/manual.tex ; \
	fi
	if [ -d "$(manual_directory)" ]; then \
		rm -f $(manual_directory)/latexmkrc ; \
	fi
	
endif
