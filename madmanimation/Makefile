all: index.html 

index.html: index.php
	php $< > $@

clean:
	rm -Rvf index.html

rebuild: clean all


