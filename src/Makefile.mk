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

## Building binaries ##

sambada_binaries = binaries/sambada binaries/supervision binaries/recode-plink binaries/recode-plink-lfmm

bin_PROGRAMS += $(sambada_binaries)

binaries_sambada_SOURCES = src/mainSambada.cpp \
	src/RegressionLogistique-io.cpp \
	src/RegressionLogistique-calc.cpp \
	src/RegressionLogistique.h \
	src/Toolbox.h \
	src/Archiviste.cpp \
	src/Archiviste.h \
	src/Erreur.h \
	src/Journal.cpp \
	src/Journal.h \
	src/FluxSortie.cpp \
	src/FluxSortie.h \
	src/JournalTemporaire.cpp \
	src/JournalTemporaire.h \
	src/Chronometre.cpp \
	src/Chronometre.h \
	src/Duree.cpp \
	src/Duree.h \
	src/LecteurCheminAcces.cpp \
    src/LecteurCheminAcces.h
binaries_sambada_CPPFLAGS = -I $(top_srcdir)/ext/scythestat-1.0.3/scythestat \
	-I $(top_srcdir)/ext/scythestat-1.0.3/scythestat/rng \
	-I $(top_srcdir)/ext/shapelib-1.3.0
binaries_sambada_CXXFLAGS = -D SCYTHE_COMPILE_DIRECT
binaries_sambada_LDADD = libshp.a libintermediate.a

binaries_supervision_SOURCES = src/mainSupervision.cpp \
	src/Supervision.cpp \
	src/Supervision.h \
	src/RegressionLogistique.h \
	src/Toolbox.h \
	src/Archiviste.cpp \
	src/Archiviste.h \
	src/Erreur.h
binaries_supervision_CPPFLAGS = -I $(top_srcdir)/ext/scythestat-1.0.3/scythestat \
	-I $(top_srcdir)/ext/scythestat-1.0.3/scythestat/rng
binaries_supervision_CXXFLAGS = -D SCYTHE_COMPILE_DIRECT
binaries_supervision_LDADD = libintermediate.a

binaries_recode_plink_SOURCES = src/recodePlink.cpp \
	src/RegressionLogistique.h \
	src/Toolbox.h \
	src/Erreur.h
binaries_recode_plink_CPPFLAGS = -I $(top_srcdir)/ext/scythestat-1.0.3/scythestat \
	-I $(top_srcdir)/ext/scythestat-1.0.3/scythestat/rng
binaries_recode_plink_CXXFLAGS = -D SCYTHE_COMPILE_DIRECT
binaries_recode_plink_LDADD = libintermediate.a

binaries_recode_plink_lfmm_SOURCES = src/recodeLFMM.cpp \
	src/RegressionLogistique.h \
	src/Toolbox.h \
	src/Erreur.h
binaries_recode_plink_lfmm_CPPFLAGS = -I $(top_srcdir)/ext/scythestat-1.0.3/scythestat \
	-I $(top_srcdir)/ext/scythestat-1.0.3/scythestat/rng
binaries_recode_plink_lfmm_CXXFLAGS = -D SCYTHE_COMPILE_DIRECT
binaries_recode_plink_lfmm_LDADD = libintermediate.a

noinst_LIBRARIES += libshp.a libintermediate.a

libintermediate_a_SOURCES = src/RegressionLogistique-bienvenue.cpp \
	src/RegressionLogistique.h \
	src/Toolbox.cpp \
	src/Toolbox.h \
	src/Erreur.cpp \
	src/Erreur.h
libintermediate_a_CPPFLAGS = -I $(top_srcdir)/ext/scythestat-1.0.3/scythestat \
	-I $(top_srcdir)/ext/scythestat-1.0.3/scythestat/rng
libintermediate_a_CXXFLAGS = -D SCYTHE_COMPILE_DIRECT

libshp_a_SOURCES = ext/shapelib-1.3.0/shpopen.c \
	ext/shapelib-1.3.0/dbfopen.c \
	ext/shapelib-1.3.0/safileio.c \
	ext/shapelib-1.3.0/shptree.c \
	ext/shapelib-1.3.0/shapefil.h

## Building archive ##
archive_binaries_basename = $(archive_basename)/binaries

create-archive-binaries-folder:
	mkdir -p $(archive_binaries_basename)

binary-archive-local-binaries: $(sambada_binaries) create-archive-binaries-folder
	cp $(sambada_binaries) $(archive_binaries_basename)

## Cleaning ##
clean-local-src:
	$(RM) -rf $(addsuffix .dSYM , $(sambada_binaries))
	$(RM) $(gcov_filename_suffixes)
