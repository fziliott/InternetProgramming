SMARTY_DIST_URL=https://github.com/smarty-php/smarty/archive/v3.1.27.tar.gz

.PHONY: 

smarty: smarty.tar.gz
	[ -d smarty ] || (tar -zxvf $< && mv smarty-*/libs smarty && rm -rf smarty-*)
	mkdir -p smarty/templates
	mkdir -p smarty/templates_c
	mkdir -p smarty/cache
	mkdir -p smarty/configs
	chmod 777 smarty/templates_c
	chmod 777 smarty/cache

smarty.tar.gz:
	wget $(SMARTY_DIST_URL) -O smarty.tar.gz

