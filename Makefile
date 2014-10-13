all: kompakt.xml md5sum.xml

kompakt.xml: kompakt.uxf
	sed -f header.sed $< > $@

md5sum.xml: md5sum.uxf
	sed -f header.sed $< > $@


