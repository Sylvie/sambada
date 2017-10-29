manual_filename = manual.tex
manual_directory := manual
manual_sources := $(top_srcdir)/$(manual_directory)/$(manual_filename) $(top_srcdir)/$(manual_directory)/manual.bib
manual_output_filename := sambadoc
manual_pdf := $(builddir)/$(manual_directory)/$(manual_output_filename).pdf

EXTRA_DIST += $(manual_sources) $(top_srcdir)/$(manual_directory)/latexmkrc_template $(builddir)/$(manual_directory)/latexmkrc
BUILT_SOURCES += $(manual_directory)/latexmkrc

if HAVE_LATEXMK
EXTRA_DIST += $(manual_pdf)
endif

LATEXMK_BUILD = $(LATEXMK) -r $(builddir)/$(manual_directory)/latexmkrc -output-directory=$(manual_directory) -jobname=$(manual_output_filename)
LATEXMK_CLEAN = $(LATEXMK) -r ../$(builddir)/$(manual_directory)/latexmkrc -jobname=$(manual_output_filename)

prepare_manual_directory:
	if [ "$(top_srcdir)" != "$(top_builddir)" ]; then \
		mkdir -p $(manual_directory) && \
		if test $(top_srcdir)/$(manual_directory)/manual.bib -nt $(manual_directory)/manual.bib; then \
			cp -f $(top_srcdir)/$(manual_directory)/manual.bib $(manual_directory); \
		fi && \
		if test $(top_srcdir)/$(manual_directory)/manual.tex -nt $(manual_directory)/manual.tex; then \
			cp -f $(top_srcdir)/$(manual_directory)/manual.tex $(manual_directory); \
		fi \
	fi

prepare_latexmkrc_file: | prepare_manual_directory
	sed "s/%versionNumber%/$(VERSION)/g; s/%releaseDate%/$(sambada_releasedate)/g" $(top_srcdir)/$(manual_directory)/latexmkrc_template > $(builddir)/$(manual_directory)/latexmkrc ;

$(manual_directory)/latexmkrc: prepare_latexmkrc_file

pdf-local-manual: $(manual_pdf) $(manual_directory)/latexmkrc

$(manual_directory)/$(manual_output_filename).pdf : $(top_srcdir)/$(manual_directory)/$(manual_filename) $(manual_directory)/latexmkrc
	$(LATEXMK_BUILD) -pdf $<

%.pdf : %.tex $(manual_directory)/latexmkrc
	$(LATEXMK_BUILD) -pdf $<

binary-archive-local-manual:
	cp -f $(manual_pdf) $(archive_basename)

clean-local-manual: prepare_latexmkrc_file
	if [ -d "$(manual_directory)" ] && [ -e "$(manual_directory)/$(manual_filename)" ] ; then \
		cd $(manual_directory) && \
		pwd && \
		$(LATEXMK_CLEAN) -c $(manual_filename) ; \
	fi

distclean-local-manual:
	if [ -d "$(manual_directory)" ] && [ -e "$(manual_directory)/$(manual_filename)" ]; then \
		cd $(manual_directory) && \
		$(LATEXMK_CLEAN) -C $(manual_filename) ; \
	fi
	if [ -d "$(manual_directory)" ] && [ "$(top_srcdir)" != "$(top_builddir)" ]; then \
		rm -f $(manual_directory)/manual.bib ; \
		rm -f $(manual_directory)/manual.tex ; \
	fi
	if [ -d "$(manual_directory)" ]; then \
		rm -f $(manual_directory)/latexmkrc ; \
	fi
