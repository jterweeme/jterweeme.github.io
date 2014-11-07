all: index.html

index.html: index.php
	@php $< > $@

clean:
	@rm -Rf index.html

rebuild: clean all


