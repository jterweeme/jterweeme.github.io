
%.html: %.php
	@php $< > $@
	@xmllint --noout --schema ../xhtml.xsd $@

.PHONY: all

all: index.html whatis.html whereis.html whois.html

index.html: index.php
whatis.html: whatis.php
whereis.html: whereis.php
whois.html: whois.php

clean:
	@rm -Rf index.html whois.html whereis.html whatis.html

rebuild: clean all



