all: index.html animatie.html

index.html: index.php
	php $< > $@

animatie.html: animatie.php
	php $< > $@

clean:
	rm -Rvf index.html animatie.html

rebuild: clean all


