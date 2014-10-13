all: index.html kompakt.xml md5sum.xml

index.html: index.php
	php $< > $@

kompakt.xml: kompakt.uxf
	sed -f header.sed $< > $@

md5sum.xml: md5sum.uxf
	sed -f header.sed $< > $@


