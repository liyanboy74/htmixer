example-dir = $(dir $(wildcard ./*/Makefile))

default: build

build:
	@for example in $(example-dir);do cd $$example;make;cd ..;done
