################################################################
# website (without the rest of the docs)

################################################################
#####  SECURITY -- change these values for lilypond.org ########
################################################################
### for lilypond.org (comment out in git)
top-src-dir=$(HOME)/src/lilypond/
trusted-dir=$(HOME)/lilypond/trusted-scripts/

### for normal git (comment out on lilypond.org)
script-dir=$(top-src-dir)/scripts/build/
texi2html-init-file=$(top-src-dir)/Documentation/lilypond-texi2html.init
TEXI2HTML=texi2html
### for lilypond.org (comment out in git)
#script-dir=$(trusted-dir)
#texi2html-init-file=$(top-src-dir)/Documentation/lilypond-texi2html.init
#TEXI2HTML=$(HOME)/usr/bin/texi2html


################################################################

EXTRACT_TEXI_FILENAMES=python $(script-dir)/extract_texi_filenames.py
CREATE_VERSION=python $(script-dir)/create-version-itexi.py
OUT=out-website
EXAMPLES=$(HOME)/media/examples/
PICTURES=$(HOME)/media/pictures


# don't include web
MANUALS=$(wildcard $(top-src-dir)/Documentation/*.tely)
MANUALS+=$(top-src-dir)/Documentation/contributor.texi

website-test:
	echo $(TEXI2HTML)

website-version:
	mkdir -p $(OUT)
	$(CREATE_VERSION) $(top-src-dir) > $(OUT)/version.itexi

website-xrefs: website-version
	$(EXTRACT_TEXI_FILENAMES) -I $(top-src-dir)/Documentation/ \
		-I $(OUT) -o $(OUT) --split=node \
		$(top-src-dir)/Documentation/web.texi
	$(foreach manual, $(MANUALS), \
		$(EXTRACT_TEXI_FILENAMES) -I $(top-src-dir)/Documentation/ \
		-I $(OUT) -o $(OUT) $(manual) && ) :

website-texinfo: website-version website-xrefs
	SRC_DIR=$(top-src-dir)/Documentation/ \
		PERL_UNICODE=SD $(TEXI2HTML) --prefix=index \
		--split=section \
		--I=$(top-src-dir)/Documentation/ \
		--I=$(OUT) \
		--init-file=$(texi2html-init-file) \
		-D web_version \
		--output=$(OUT)/website/ \
		$(top-src-dir)/Documentation/web.texi

website-css:
	cp $(top-src-dir)/Documentation/css/*.css $(OUT)/website/

website-pictures:
	cp -r $(PICTURES) $(OUT)/website/
	ln -sf website/pictures $(OUT)/pictures

website-examples:
	cp -r $(EXAMPLES) $(OUT)/website/


website: website-texinfo website-css website-pictures website-examples

