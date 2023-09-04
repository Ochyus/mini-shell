default: clean build

clean: mini-shell
	rm mini-shell

build: mini-shell.c
	gcc mini-shell.c -o mini-shell