.PHONY: all config web hotel paper clean

all: config web/smarty ccgi/libccgi.a paper hotel web

config: web/config.php paper/config.h

%/config.php: config.ini
	python confgen.py php $@

%/config.h: config.ini
	python confgen.py c $@

web/smarty:
	make -C web smarty

ccgi/libccgi.a:
	make -C ccgi

paper:
		make -C paper build
hotel:
		make -C hotel build
web: 
		make -C web/cgi-bin build

clean:
		make -C paper clean
		make -C hotel clean
		make -C web/cgi-bin clean
