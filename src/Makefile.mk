##########################################################################
# Copyright (©) 2011-2019 EPFL (Ecole Polytechnique fédérale de Lausanne)
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

## Building binaries ##

sambada_binaries = binaries/sambada binaries/supervision binaries/recode-plink binaries/recode-plink-lfmm

bin_PROGRAMS += $(sambada_binaries)

binaries_sambada_SOURCES = src/mainSambada.cpp \
	src/RegressionLogistique-io.cpp \
	src/RegressionLogistique-calc.cpp \
	src/RegressionLogistique-as.cpp \
	src/RegressionLogistique.hpp \
	src/Toolbox.hpp \
	src/Archiviste.cpp \
	src/Archiviste.hpp \
	src/Erreur.hpp \
	src/Journal.cpp \
	src/Journal.hpp \
	src/FluxSortie.cpp \
	src/FluxSortie.hpp \
	src/JournalTemporaire.cpp \
	src/JournalTemporaire.hpp \
	src/Chronometre.cpp \
	src/Chronometre.hpp \
	src/Duree.cpp \
	src/Duree.hpp \
	src/LecteurCheminAcces.cpp \
    src/LecteurCheminAcces.hpp
binaries_sambada_CPPFLAGS = -I $(top_srcdir)/ext/scythestat-1.0.3/scythestat \
	-I $(top_srcdir)/ext/scythestat-1.0.3/scythestat/rng \
	-I $(top_srcdir)/ext/shapelib-1.3.0 \
	-I $(top_srcdir)/src
binaries_sambada_CXXFLAGS = -D SCYTHE_COMPILE_DIRECT
binaries_sambada_LDADD = \
	src/libshp.a \
	src/libintermediate.a \
	src/common/libcommon.a \
	src/histograms/libhistograms.a \
	src/modeles/libmodeles.a \
	src/variables/libvariables.a

binaries_supervision_SOURCES = src/mainSupervision.cpp \
	src/Supervision.cpp \
	src/Supervision.hpp \
	src/RegressionLogistique.hpp \
	src/Toolbox.hpp \
	src/Archiviste.cpp \
	src/Archiviste.hpp \
	src/Erreur.hpp
binaries_supervision_CPPFLAGS = -I $(top_srcdir)/ext/scythestat-1.0.3/scythestat \
	-I $(top_srcdir)/ext/scythestat-1.0.3/scythestat/rng \
	-I $(top_srcdir)/src

binaries_supervision_CXXFLAGS = -D SCYTHE_COMPILE_DIRECT
binaries_supervision_LDADD = src/libintermediate.a

binaries_recode_plink_SOURCES = src/recodePlink.cpp \
	src/RegressionLogistique.hpp \
	src/Toolbox.hpp \
	src/Erreur.hpp
binaries_recode_plink_CPPFLAGS = -I $(top_srcdir)/ext/scythestat-1.0.3/scythestat \
	-I $(top_srcdir)/ext/scythestat-1.0.3/scythestat/rng \
	-I $(top_srcdir)/src
binaries_recode_plink_CXXFLAGS = -D SCYTHE_COMPILE_DIRECT
binaries_recode_plink_LDADD = src/libintermediate.a

binaries_recode_plink_lfmm_SOURCES = src/recodeLFMM.cpp \
	src/RegressionLogistique.hpp \
	src/Toolbox.hpp \
	src/Erreur.hpp
binaries_recode_plink_lfmm_CPPFLAGS = -I $(top_srcdir)/ext/scythestat-1.0.3/scythestat \
	-I $(top_srcdir)/ext/scythestat-1.0.3/scythestat/rng \
	-I $(top_srcdir)/src

binaries_recode_plink_lfmm_CXXFLAGS = -D SCYTHE_COMPILE_DIRECT
binaries_recode_plink_lfmm_LDADD = src/libintermediate.a

noinst_LIBRARIES += src/libshp.a src/libintermediate.a

src_libintermediate_a_SOURCES = src/RegressionLogistique-bienvenue.cpp \
	src/RegressionLogistique.hpp \
	src/Toolbox.cpp \
	src/Toolbox.hpp \
	src/Erreur.cpp \
	src/Erreur.hpp
src_libintermediate_a_CPPFLAGS = -I $(top_srcdir)/ext/scythestat-1.0.3/scythestat \
	-I $(top_srcdir)/ext/scythestat-1.0.3/scythestat/rng \
	-I $(top_srcdir)/src
src_libintermediate_a_CXXFLAGS = -D SCYTHE_COMPILE_DIRECT

src_libshp_a_SOURCES = ext/shapelib-1.3.0/shpopen.c \
	ext/shapelib-1.3.0/dbfopen.c \
	ext/shapelib-1.3.0/safileio.c \
	ext/shapelib-1.3.0/shptree.c \
	ext/shapelib-1.3.0/shapefil.h

include src/common/Makefile.mk
include src/histograms/Makefile.mk
include src/modeles/Makefile.mk
include src/variables/Makefile.mk

## Building archive ##
archive_binaries_basename = $(archive_basename)/binaries

create-archive-binaries-folder:
	mkdir -p $(archive_binaries_basename)

binary-archive-local-binaries: $(sambada_binaries) create-archive-binaries-folder
	cp $(sambada_binaries) $(archive_binaries_basename)

## Cleaning ##
clean-local-src: clean-local-src-common clean-local-src-histograms clean-local-src-modeles clean-local-src-variables
	$(call clean_extra_generated_files, src/)
	$(call clean_extra_generated_files, binaries/)
