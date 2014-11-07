all: index.html md5sum.html kompakt.html rm.html
	@make -C madmanimation all
	@make -C yahtzee all

index.html: index.php
	@php $< > $@
	@xmllint --noout --schema xhtml.xsd $@

md5sum.html: md5sum.php omelet.php md5sum.uxf
	@php $< > $@
	@xmllint --noout --schema xhtml.xsd $@

kompakt.html: kompakt.php omelet.php kompakt.uxf
	@php $< > $@
	@xmllint --noout --schema xhtml.xsd $@

rm.html: rm.php omelet.php rm.uxf
	@php $< > $@
	@xmllint --noout --schema xhtml.xsd $@

clean:
	@rm -Rf index.html md5sum.html kompakt.html kompakt.xml md5sum.xml rm.html
	@make -C madmanimation clean
	@make -C yahtzee clean

rebuild: clean all



