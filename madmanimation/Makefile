all: index.html animatie.html

index.html: index.php
	@php $< > $@

animatie.html: animatie.php
	@php $< > $@

clean:
	@rm -Rf index.html animatie.html

rebuild: clean all


