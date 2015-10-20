#!/usr/bin/env python

import ConfigParser, os, sys

CONF='config.ini'

def dump_conf_php(config):
	lines = []
	for section in config.sections():
		for option in config.options(section):
			varname = ('$%s_%s' % (section, option)).upper()
			varvalue = config.get(section, option)
			try:
				lines.append('%s=%d;' % (varname, int(varvalue)))
			except ValueError:
				lines.append('%s="%s";' % (varname, varvalue))
	lines.insert(0, '<?php')
	lines.append('?>')
	lines.append('')
	return '\n'.join(lines)

def dump_conf_c(config):
	lines = []
	for section in config.sections():
		for option in config.options(section):
			varname = ('%s_%s' % (section, option)).upper()
			varvalue = config.get(section, option)
			try:
				lines.append('#define %s %d' % (varname, int(varvalue)))
			except ValueError:
				lines.append('#define %s "%s"' % (varname, varvalue))
	lines.insert(0, '#ifndef __ASSIGNMENT4__')
	lines.insert(1, '#define __ASSIGNMENT4__')
	lines.append('#endif')
	lines.append('')
	return '\n'.join(lines)

if __name__ == '__main__':
	config = ConfigParser.ConfigParser()
	config.read(CONF)
	
	if len(sys.argv) != 3:
		sys.exit1()
	elif sys.argv[1] == 'c':
		with open(sys.argv[2], 'w') as f:
			f.write(dump_conf_c(config))
	elif sys.argv[1] == 'php':
		with open(sys.argv[2], 'w') as f:
			f.write(dump_conf_php(config))
