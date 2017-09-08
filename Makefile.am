## Process this file with automake to produce Makefile.in

EXTRA_DIST = $(top_srcdir)/.version
BUILT_SOURCES = $(top_srcdir)/.version
$(top_srcdir)/.version:
	echo $(VERSION) > $@-t && mv $@-t $@
dist-hook:
	echo $(VERSION) > $(distdir)/.tarball-version

#subdirs = ext/scythestat-1.0.3 ext/shapelib-1.3.0

bin_PROGRAMS = sambada supervision recode-plink recode-plink-lfmm
sambada_SOURCES = src/mainSambada.cpp \
	src/RegressionLogistique-io.cpp \
	src/RegressionLogistique-calc.cpp \
	src/RegressionLogistique-bienvenue.cpp \
	src/RegressionLogistique.h \
	src/Toolbox.cpp \
	src/Toolbox.h \
	src/Archiviste.cpp \
	src/Archiviste.h \
	src/Erreur.cpp \
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
sambada_LDADD = libshp.a

supervision_SOURCES = src/mainSupervision.cpp \
	src/Supervision.cpp \
	src/Supervision.h \
	src/RegressionLogistique-bienvenue.cpp \
	src/RegressionLogistique.h \
	src/Toolbox.cpp \
	src/Toolbox.h \
	src/Archiviste.cpp \
	src/Archiviste.h \
	src/Erreur.cpp \
	src/Erreur.h
supervision_CPPFLAGS = -I $(srcdir)/ext/scythestat-1.0.3/scythestat \
	-I $(srcdir)/ext/scythestat-1.0.3/scythestat/rng
supervision_CXXFLAGS = -D SCYTHE_COMPILE_DIRECT

recode_plink_SOURCES = src/recodePlink.cpp \
	src/RegressionLogistique-bienvenue.cpp \
	src/RegressionLogistique.h \
	src/Toolbox.cpp \
	src/Toolbox.h \
	src/Erreur.cpp \
	src/Erreur.h
recode_plink_CPPFLAGS = -I $(srcdir)/ext/scythestat-1.0.3/scythestat \
	-I $(srcdir)/ext/scythestat-1.0.3/scythestat/rng
recode_plink_CXXFLAGS = -D SCYTHE_COMPILE_DIRECT

recode_plink_lfmm_SOURCES = src/recodeLFMM.cpp \
	src/RegressionLogistique-bienvenue.cpp \
	src/RegressionLogistique.h \
	src/Toolbox.cpp \
	src/Toolbox.h \
	src/Erreur.cpp \
	src/Erreur.h
recode_plink_lfmm_CPPFLAGS = -I $(srcdir)/ext/scythestat-1.0.3/scythestat \
	-I $(srcdir)/ext/scythestat-1.0.3/scythestat/rng
recode_plink_lfmm_CXXFLAGS = -D SCYTHE_COMPILE_DIRECT

noinst_LIBRARIES = libshp.a
libshp_a_SOURCES = ext/shapelib-1.3.0/shpopen.c \
	ext/shapelib-1.3.0/dbfopen.c \
	ext/shapelib-1.3.0/safileio.c \
	ext/shapelib-1.3.0/shptree.c \
	ext/shapelib-1.3.0/shapefil.h

pdf-local: pdf-local-manual

archive_basename = $(builddir)/$(PACKAGE)-$(VERSION)-$(system_name)
archive_binaries_basename = $(archive_basename)/binaries

create-archive-folder:
	mkdir -p $(archive_basename)
	mkdir -p $(archive_binaries_basename)

binary-archive: all pdf create-archive-folder
	$(MAKE) binary-archive-local-manual
	cp $(bin_PROGRAMS) $(archive_binaries_basename)
	tar -zcvf $(archive_basename).tar.gz $(archive_basename)

MOSTLYCLEANFILES =
CLEANFILES =
DISTCLEANFILES =
MAINTAINERCLEANFILES =
REPOCLEANFILES = $(addprefix $(srcdir)/, aclocal.m4 configure config.guess config.sub depcomp install-sh Makefile.in missing)

clean-local: #clean-local-manual

distclean-local: distclean-local-manual

repo-clean: maintainer-clean
	rm -rf $(REPOCLEANFILES)

#include manual/Makefile.mk
