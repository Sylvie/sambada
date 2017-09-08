bin_PROGRAMS = sambada supervision recode-plink recode-plink-lfmm

sambada_SOURCES = src/mainSambada.cpp \
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
sambada_CPPFLAGS = -I $(srcdir)/ext/scythestat-1.0.3/scythestat \
-I $(srcdir)/ext/scythestat-1.0.3/scythestat/rng \
-I $(srcdir)/ext/shapelib-1.3.0
sambada_CXXFLAGS = -D SCYTHE_COMPILE_DIRECT
sambada_LDADD = libshp.a libintermediate.a

supervision_SOURCES = src/mainSupervision.cpp \
	src/Supervision.cpp \
	src/Supervision.h \
	src/RegressionLogistique.h \
	src/Toolbox.h \
	src/Archiviste.cpp \
	src/Archiviste.h \
	src/Erreur.h
supervision_CPPFLAGS = -I $(srcdir)/ext/scythestat-1.0.3/scythestat \
	-I $(srcdir)/ext/scythestat-1.0.3/scythestat/rng
supervision_CXXFLAGS = -D SCYTHE_COMPILE_DIRECT
supervision_LDADD = libintermediate.a

recode_plink_SOURCES = src/recodePlink.cpp \
	src/RegressionLogistique.h \
	src/Toolbox.h \
	src/Erreur.h
recode_plink_CPPFLAGS = -I $(srcdir)/ext/scythestat-1.0.3/scythestat \
	-I $(srcdir)/ext/scythestat-1.0.3/scythestat/rng
recode_plink_CXXFLAGS = -D SCYTHE_COMPILE_DIRECT
recode_plink_LDADD = libintermediate.a

recode_plink_lfmm_SOURCES = src/recodeLFMM.cpp \
	src/RegressionLogistique.h \
	src/Toolbox.h \
	src/Erreur.h
recode_plink_lfmm_CPPFLAGS = -I $(srcdir)/ext/scythestat-1.0.3/scythestat \
	-I $(srcdir)/ext/scythestat-1.0.3/scythestat/rng
recode_plink_lfmm_CXXFLAGS = -D SCYTHE_COMPILE_DIRECT
recode_plink_lfmm_LDADD = libintermediate.a

noinst_LIBRARIES = libshp.a libintermediate.a

libintermediate_a_SOURCES = src/RegressionLogistique-bienvenue.cpp \
	src/RegressionLogistique.h \
	src/Toolbox.cpp \
	src/Toolbox.h \
	src/Erreur.cpp \
	src/Erreur.h
libintermediate_a_CPPFLAGS = -I $(srcdir)/ext/scythestat-1.0.3/scythestat \
	-I $(srcdir)/ext/scythestat-1.0.3/scythestat/rng
libintermediate_a_CXXFLAGS = -D SCYTHE_COMPILE_DIRECT

libshp_a_SOURCES = ext/shapelib-1.3.0/shpopen.c \
	ext/shapelib-1.3.0/dbfopen.c \
	ext/shapelib-1.3.0/safileio.c \
	ext/shapelib-1.3.0/shptree.c \
	ext/shapelib-1.3.0/shapefil.h
