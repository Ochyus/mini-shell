default: clean build

clean: shell
	rm shell

build: shell.c
	gcc shell.c -o shell