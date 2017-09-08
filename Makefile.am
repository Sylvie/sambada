## Process this file with automake to produce Makefile.in

EXTRA_DIST = $(top_srcdir)/.version
BUILT_SOURCES = $(top_srcdir)/.version
$(top_srcdir)/.version:
	echo $(VERSION) > $@-t && mv $@-t $@
dist-hook:
	echo $(VERSION) > $(distdir)/.tarball-version

include src/Makefile.mk

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
REPOCLEANFILES = $(addprefix $(srcdir)/, aclocal.m4 compile configure config.guess config.sub depcomp install-sh Makefile.in missing)

clean-local: #clean-local-manual

distclean-local: #distclean-local-manual

repo-clean: maintainer-clean
	rm -rf $(REPOCLEANFILES)

#include manual/Makefile.mk
