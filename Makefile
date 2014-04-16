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

$(BUILDDIR)/RecodePlink$(NUMVERSION)$(TAGSYS)$(SUFFIXE_SYS): $(addprefix $(BUILDDIR)/, recodePlink.o) 
	$(LINK.SCYTHE) -o $@ $^

$(BUILDDIR)/RecodePlink_LFMM$(NUMVERSION)$(TAGSYS)$(SUFFIXE_SYS): $(addprefix $(BUILDDIR)/, recodeLFMM.o) 
	$(LINK.SCYTHE) -o $@ $^

$(BUILDDIR)/libshp.a:	$(addprefix $(BUILDDIR)/$(SHPDIR)/, $(LIBOBJ))
	$(AR) -r -s $@ $^

$(BUILDDIR)/%.o: %.cpp
	$(COMPILE.SCYTHE) -o $@ $<

$(BUILDDIR)/$(SHPDIR)/%.o: %.c
	$(COMPILE.SHAPELIB) -o $@ $<

clean :			
	-@/bin/rm -fr $(BUILDDIR) VERSION 
#	-@/bin/rm -fr *.o *.a $(BUILDDIR)/$(SHPDIR)/*.o $(BUILDDIR)/*.o $(BUILDDIR)/*.a

clean-obj:
	-@/bin/rm -fr $(BUILDDIR)/*.o VERSION 	

include $(subst .c,.d,$(SOURCES)) $(subst .cpp,.d,$(SOURCES))    

%.d: %.c  
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \  
		sed 's,\($*\)\.o[ :]*,.o $@ : ,g' < $@.$$$$ > $@; \  		
		rm -f $@.$$$$

