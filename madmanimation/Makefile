all: index.html animatie.html

index.html: index.php
	@php $< > $@
	@xmllint --noout --schema ../xhtml.xsd $@

animatie.html: animatie.php
	@php $< > $@
	@xmllint --noout --schema ../xhtml.xsd $@

clean:
	@rm -Rf index.html animatie.html

rebuild: clean all


