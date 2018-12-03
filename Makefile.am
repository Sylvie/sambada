##########################################################################
# Copyright (©) 2011-2018 EPFL (Ecole Polytechnique fédérale de Lausanne)
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

## Process this file with automake to produce Makefile.in

## Version and release date ##

EXTRA_DIST = $(top_srcdir)/.version $(top_srcdir)/.releasedate
BUILT_SOURCES = $(top_srcdir)/.version $(top_srcdir)/.releasedate VERSION
$(top_srcdir)/.version:
	echo $(VERSION) > $@-t && mv $@-t $@
$(top_srcdir)/.releasedate:
	echo $(sambada_releasedate) > $@-t && mv $@-t $@
dist-hook:
	echo $(VERSION) > $(distdir)/.tarball-version
	echo $(sambada_releasedate) > $(distdir)/.tarball-releasedate

## Binaries ##

bin_PROGRAMS =
noinst_PROGRAMS =
noinst_LIBRARIES =
include src/Makefile.mk

EXTRA_DIST += $(top_srcdir)/ext/scythestat-1.0.3 $(top_srcdir)/ext/shapelib-1.3.0

## Tests ##

AM_CPPFLAGS = -I$(top_srcdir)/src

TESTS =

check-local: check-local-test

include test/Makefile.mk

## Manual ##

pdf-local: pdf-local-manual

include manual/Makefile.mk

## Examples ##

dist_pkgdata_DATA =
include examples/Makefile.mk

## Scripts ##

include scripts/Makefile.mk

## General information ##
general_info_files = AUTHORS CHANGELOG COPYING README
dist_doc_DATA = $(general_info_files) VERSION

VERSION: $(top_srcdir)/.version $(top_srcdir)/.releasedate
	echo Version: $(VERSION) > $@-t && \
	echo Release date: $(sambada_releasedate) >> $@-t && \
	mv $@-t $@

## Building archive ##

archive_basename = $(builddir)/$(PACKAGE)-$(VERSION)-$(system_name)

create-archive-folder:
	mkdir -p $(archive_basename)

binary-archive: all pdf create-archive-folder
	$(MAKE) binary-archive-local-binaries
	$(MAKE) binary-archive-local-manual
	$(MAKE) binary-archive-local-examples
	$(MAKE) binary-archive-local-scripts
	cp $(addprefix $(top_srcdir)/, $(general_info_files)) $(archive_basename)
	cp VERSION $(archive_basename)
	tar -zcvf $(archive_basename).tar.gz $(archive_basename)

## Cleaning ##

MOSTLYCLEANFILES =
CLEANFILES =
DISTCLEANFILES =
MAINTAINERCLEANFILES =
REPOCLEANFILES = $(addprefix $(srcdir)/, aclocal.m4 compile configure config.guess config.sub depcomp install-sh Makefile.in missing)

gcov_filename_suffixes = **/*.gcov **/*.gcno **/*.gcda

clean-local: clean-local-src clean-local-test clean-local-manual

distclean-local: distclean-local-manual

repo-clean: maintainer-clean
	rm -rf $(REPOCLEANFILES)

define clean_extra_generated_files
    suffixes := .dSYM $(gcov_filename_suffixes)
    $(foreach suffixe, $(suffixes), $(RM) -rf $(addsuffix $(suffixe) , $(1)))
endef
