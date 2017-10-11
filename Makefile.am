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
include src/Makefile.mk

EXTRA_DIST += $(top_srcdir)/ext/scythestat-1.0.3 $(top_srcdir)/ext/shapelib-1.3.0

## Manual ##

pdf-local: pdf-local-manual

include manual/Makefile.mk

## Examples ##

dist_pkgdata_DATA =
include examples/Makefile.mk

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
	cp $(addprefix $(top_srcdir)/, $(general_info_files)) $(archive_basename)
	cp VERSION $(archive_basename)
	tar -zcvf $(archive_basename).tar.gz $(archive_basename)

## Cleaning ##

MOSTLYCLEANFILES =
CLEANFILES =
DISTCLEANFILES =
MAINTAINERCLEANFILES =
REPOCLEANFILES = $(addprefix $(srcdir)/, aclocal.m4 compile configure config.guess config.sub depcomp install-sh Makefile.in missing)

clean-local: clean-local-src clean-local-manual

distclean-local: distclean-local-manual

repo-clean: maintainer-clean
	rm -rf $(REPOCLEANFILES)


