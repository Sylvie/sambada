manual_filename = manual.tex
manual_directory := "manual/"
manual_sources := $(top_srcdir)/manual/$(manual_filename) $(top_srcdir)/manual/manual.bib
manual_pdf := $(builddir)/manual/manual.pdf

EXTRA_DIST += $(manual_sources) $(top_srcdir)/manual/latexmkrc_template $(builddir)/manual/latexmkrc
BUILT_SOURCES += manual/latexmkrc

if HAVE_LATEXMK
EXTRA_DIST += $(manual_pdf)
endif

LATEXMK_BUILD = $(LATEXMK) -r $(builddir)/manual/latexmkrc -output-directory=$(manual_directory)
LATEXMK_CLEAN = $(LATEXMK) -r ../$(builddir)/manual/latexmkrc

prepare_manual_directory:
	if [ "$(top_srcdir)" != "$(top_builddir)" ]; then \
		mkdir -p $(manual_directory) && \
		if test $(top_srcdir)/manual/manual.bib -nt $(manual_directory)/manual.bib; then \
			cp -f $(top_srcdir)/manual/manual.bib $(manual_directory); \
		fi && \
		if test $(top_srcdir)/manual/manual.tex -nt $(manual_directory)/manual.tex; then \
			cp -f $(top_srcdir)/manual/manual.tex $(manual_directory); \
		fi \
	fi

prepare_latexmkrc_file: | prepare_manual_directory
	sed "s/%versionNumber%/$(VERSION)/g; s/%releaseDate%/$(sambada_releasedate)/g" $(top_srcdir)/manual/latexmkrc_template > $(builddir)/manual/latexmkrc ;

manual/latexmkrc: prepare_latexmkrc_file

pdf-local-manual: $(manual_pdf) manual/latexmkrc

%.pdf : %.tex manual/latexmkrc
	$(LATEXMK_BUILD) -pdf $<

binary-archive-local-manual:
	cp -f $(manual_pdf) $(archive_basename)

clean-local-manual:
	if [ -d "$(manual_directory)" ] && [ -e "$(manual_directory)$(manual_filename)" ] ; then \
		cd $(manual_directory) && \
		pwd && \
		$(LATEXMK_CLEAN) -c $(manual_filename) ; \
	fi

distclean-local-manual:
	if [ -d "$(manual_directory)" ] && [ -e "$(manual_directory)$(manual_filename)" ]; then \
		cd $(manual_directory) && \
		$(LATEXMK_CLEAN) -C $(manual_filename) ; \
	fi
	if [ -d "$(manual_directory)" ] && [ "$(top_srcdir)" != "$(top_builddir)" ]; then \
		rm -f manual/manual.bib ; \
		rm -f manual/manual.tex ; \
	fi
	if [ -d "$(manual_directory)" ]; then \
		rm -f manual/latexmkrc ; \
	fi
