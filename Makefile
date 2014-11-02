all: index.html md5sum.html kompakt.html rm.html
	make -C madmanimation all

index.html: index.php
	php $< > $@

md5sum.html: md5sum.php omelet.php md5sum.uxf
	php $< > $@

kompakt.html: kompakt.php omelet.php kompakt.uxf
	php $< > $@

rm.html: rm.php omelet.php rm.uxf
	php $< > $@

#kompakt.xml: kompakt.uxf
#	sed -f header.sed $< > $@

#md5sum.xml: md5sum.uxf
#	sed -f header.sed $< > $@

clean:
	rm -Rvf index.html md5sum.html kompakt.html kompakt.xml md5sum.xml rm.html

rebuild: clean all


