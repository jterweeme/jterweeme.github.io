all: index.html

index.html: index.php
	@php $< > $@
	@xmllint --noout --schema ../xhtml.xsd $@

clean:
	@rm -Rf index.html

rebuild: clean all



