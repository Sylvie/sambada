SYS_CIBLE = NATIF
#SYS_CIBLE = WIN32
#SYS_CIBLE = WIN64

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

ifneq ($(SYS_CIBLE), Darwin)
 CPPFLAGS += -static
endif

ifeq ($(SYS_CIBLE), WIN32)
 PREFIXE_SYS = WIN32_
 ifeq ($(SYSTEM), Darwin)
  CXX = i686-w64-mingw32-g++
  CC = i686-w64-mingw32-gcc
  AR = i686-w64-mingw32-ar
  CPPFLAGS += -static
  CFLAGS += -static
  SUFFIXE_SYS = .exe
 endif

else ifeq ($(SYS_CIBLE), WIN64)
 PREFIXE_SYS = WIN64_
 ifeq ($(SYSTEM), Darwin)
  CXX = x86_64-w64-mingw32-g++
  CC = i686-w64-mingw32-gcc  
  AR = i686-w64-mingw32-ar
  SUFFIXE_SYS = .exe
 endif

else #  ->ifeq ($(SYS_CIBLE), NATIF)
 ifeq ($(SYSTEM), Darwin)
  PREFIXE_SYS = OSX_
 else
  PREFIXE_SYS = LINUX_
 endif
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

all : $(foreach var, Sambada Supervision, $(BUILDDIR)/$(PREFIXE_SYS)$(var)$(SUFFIXE_SYS)) 

$(BUILDDIR)/$(PREFIXE_SYS)Sambada$(SUFFIXE_SYS): $(addprefix $(BUILDDIR)/, mainSambada.o RegressionLogistique-io.o RegressionLogistique-calc.o Toolbox.o Archiviste.o Erreur.o libshp.a) #$(foreach var, TestSAM.o RegressionLogistique-io.o RegressionLogistique-calc.o Toolbox.o Archiviste.o Erreur.o, $(PREFIXE_SYS)$(var))
	$(LINK.SCYTHE) -o $@ $^

$(BUILDDIR)/$(PREFIXE_SYS)Supervision$(SUFFIXE_SYS): $(addprefix $(BUILDDIR)/, mainSupervision.o Supervision.o Toolbox.o Archiviste.o Erreur.o) #$(foreach var, TestSupervision.o Supervision.o Toolbox.o Archiviste.o Erreur.o, $(PREFIXE_SYS)$(var))
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
	
include $(subst .c,.d,$(SOURCES)) $(subst .cpp,.d,$(SOURCES))    

%.d: %.c  
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \  
		sed 's,\($*\)\.o[ :]*,.o $@ : ,g' < $@.$$$$ > $@; \  		
		rm -f $@.$$$$

