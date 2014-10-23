all: index.html md5sum.html kompakt.html

index.html: index.php
	php $< > $@

md5sum.html: md5sum.php
	php $< > $@

kompakt.html: kompakt.php
	php $< > $@

#kompakt.xml: kompakt.uxf
#	sed -f header.sed $< > $@

#md5sum.xml: md5sum.uxf
#	sed -f header.sed $< > $@

clean:
	rm -Rvf index.html md5sum.html kompakt.html kompakt.xml md5sum.xml

