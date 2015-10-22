.PHONY: all config

all: config web/smarty ccgi/libccgi.a

config: web/config.php paper/config.h

%/config.php: config.ini
	python confgen.py php $@

%/config.h: config.ini
	python confgen.py c $@

web/smarty:
	make -C web smarty

ccgi/libccgi.a:
	make -C ccgi
