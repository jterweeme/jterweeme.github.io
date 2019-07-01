
%.html: %.php
	@php $< > $@
	@xmllint --noout --schema xhtml.xsd $@

.PHONY: all

all: index.html md5sum.html kompakt.html ringtone.html \
rm.html simpelweb.html solitaire.html verkeer.html worst.html
	@make -C euler
	@make -C fotopuzzel all
	@make -C madmanimation all
	@make -C penny all
	@make -C yahtzee all
	@make -C yvettes all

index.html: index.php
md5sum.html: md5sum.php omelet.php md5sum.uxf
kompakt.html: kompakt.php omelet.php kompakt.uxf
ringtone.html: ringtone.php
rm.html: rm.php omelet.php rm.uxf
simpelweb.html: simpelweb.php
solitaire.html: solitaire.php
verkeer.html: verkeer.php
worst.html: worst.php

clean:
	@rm -Rf index.html md5sum.html kompakt.html kompakt.xml md5sum.xml rm.html
	@make -C euler clean
	@make -C fotopuzzel clean
	@make -C madmanimation clean
	@make -C penny clean
	@make -C yahtzee clean
	@make -C yvettes clean

rebuild: clean all



