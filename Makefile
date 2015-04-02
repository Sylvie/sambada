##########################################################################
# Copyright (©) 2011-2015 EPFL (Ecole Polytechnique fédérale de Lausanne)
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



SHELL := /bin/bash

ifndef NUMVERSION
	NUMVERSION:=-$(shell ./update-version-number.sh)
endif


SHPDIR = shapelib-1.3.0
SCYTHEDIR = scythestat-1.0.3/scythestat scythestat-1.0.3/scythestat/rng
# No object files produced from scythestat lib
BUILDDIR = binaries

REQUIRED_DIRS =	$(BUILDDIR) $(BUILDDIR)/$(SHPDIR)
_MKDIRS := $(shell for d in "$(REQUIRED_DIRS)";	\
	do											\
		[[ -d $$d ]] ||  mkdir -p $$d;			\
	done)
	
CC = gcc
CXX = g++
AR = ar

CPPFLAGS = # -ansi -pedantic -Wall
CPPFLAGS += -O3
CPPFLAGS += $(addprefix -I , $(SHPDIR) $(SCYTHEDIR) )
CXXFLAGS += $(PKG_CXXFLAGS)
# CPPFLAGS += -g
# CPPFLAGS += -pg
# CPPFLAGS += -static

# On demande le nom du systeme
SYSTEM =  $(shell (uname | cut -b 1-6))

# SYS est fourni par l'utilisateur pour le nommage des fichiers
ifndef SYS
	TAGSYS = 
else
	TAGSYS = -$(SYS)
endif

# Copie des 3 premier caractères et mise en majuscules
SUBSTR_SYS=$(shell export SYS="$(SYS)"; echo $${SYS:0:3} | tr '[:lower:]' '[:upper:]')

ifeq ($(SUBSTR_SYS), WIN)
	SUFFIXE_SYS = .exe
else
	SUFFIXE_SYS = 
endif


ifneq ($(SYSTEM), Darwin)
 CPPFLAGS += -static
endif


# bibliothèques générales
LDFLAGS =	
LDLIBS  = 

PKG_CXXFLAGS = -D SCYTHE_COMPILE_DIRECT

# Bibliothèques spécifiques (OpenGL et glut ici) :

# On demande le nom du systeme
# SYSTEM =  $(shell (uname | cut -b 1-6))

#	# Puis on choisit les bibliotheques a inclure
#	ifeq ($(SYSTEM), Darwin)
#	 GLUTLDFLAGS =
#	 GLUTLDLIBS  = -framework OpenGL -framework GLUT
#	else 
#	 # On pourrait rajouter un niveau de choix ici (p. ex Linux - CYGWIN)
#	 # ifeq ($(SYSTEM), Linux)
#	  GLUTLDFLAGS  = -L/usr/X11R6/lib/
#	  GLUTLDLIBS   = -lglut -lGLU -lGL -lXmu -lXi
#	 # else # qqch pour l'autre choix
#	 # endif
#	endif

#	LINK.GLUT = $(LINK.cc) $(GLUTLDFLAGS)

vpath %.cpp = $(SCYTHEDIR)
vpath %.c = $(SHPDIR)

COMPILE.SCYTHE = $(COMPILE.cc) #$(PKG_CXXFLAGS)
COMPILE.SHAPELIB = $(COMPILE.c)
LINK.SCYTHE = $(LINK.cc) #$(PKG_CXXFLAGS)
LINK.SHAPELIB = $(LINK.c)


#LIBOBJ	= $(foreach var, shpopen.o dbfopen.o safileio.o shptree.o, $(SHPDIR)/$(var) )
LIBOBJ	= shpopen.o dbfopen.o safileio.o shptree.o

all : $(foreach var, Sambada Supervision RecodePlink RecodePlink_LFMM, $(BUILDDIR)/$(var)$(NUMVERSION)$(TAGSYS)$(SUFFIXE_SYS)) 


$(BUILDDIR)/Sambada$(NUMVERSION)$(TAGSYS)$(SUFFIXE_SYS): $(addprefix $(BUILDDIR)/, mainSambada.o RegressionLogistique-io.o RegressionLogistique-calc.o Toolbox.o Archiviste.o Erreur.o libshp.a)  #$(foreach var, TestSAM.o RegressionLogistique-io.o RegressionLogistique-calc.o Toolbox.o Archiviste.o Erreur.o, $(PREFIXE_SYS)$(var))
	$(LINK.SCYTHE) -o $@ $^

$(BUILDDIR)/Supervision$(NUMVERSION)$(TAGSYS)$(SUFFIXE_SYS): $(addprefix $(BUILDDIR)/, mainSupervision.o Supervision.o Toolbox.o Archiviste.o Erreur.o)  #$(foreach var, TestSupervision.o Supervision.o Toolbox.o Archiviste.o Erreur.o, $(PREFIXE_SYS)$(var))
	$(LINK.SCYTHE) -o $@ $^

$(BUILDDIR)/RecodePlink$(NUMVERSION)$(TAGSYS)$(SUFFIXE_SYS): $(addprefix $(BUILDDIR)/, Toolbox.o Erreur.o recodePlink.o) 
	$(LINK.SCYTHE) -o $@ $^

$(BUILDDIR)/RecodePlink_LFMM$(NUMVERSION)$(TAGSYS)$(SUFFIXE_SYS): $(addprefix $(BUILDDIR)/, Toolbox.o Erreur.o recodeLFMM.o) 
	$(LINK.SCYTHE) -o $@ $^

$(BUILDDIR)/libshp.a:	$(addprefix $(BUILDDIR)/$(SHPDIR)/, $(LIBOBJ))
	$(AR) -r -s $@ $^

$(BUILDDIR)/%.o: %.cpp
	$(COMPILE.SCYTHE) -o $@ $<

$(BUILDDIR)/$(SHPDIR)/%.o: %.c
	$(COMPILE.SHAPELIB) -o $@ $<

clean :			
	-@/bin/rm -fr $(BUILDDIR) 
#	-@/bin/rm -fr *.o *.a $(BUILDDIR)/$(SHPDIR)/*.o $(BUILDDIR)/*.o $(BUILDDIR)/*.a

clean-obj:
	-@/bin/rm -fr $(BUILDDIR)/*.o 	

include $(subst .c,.d,$(SOURCES)) $(subst .cpp,.d,$(SOURCES))    

%.d: %.c  
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \  
		sed 's,\($*\)\.o[ :]*,.o $@ : ,g' < $@.$$$$ > $@; \  		
		rm -f $@.$$$$

