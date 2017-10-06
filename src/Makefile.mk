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
	src/Duree.h
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

noinst_LIBRARIES = libshp.a libintermediate.a

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
