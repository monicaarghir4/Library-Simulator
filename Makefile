build: 
	gcc -m32 *.c -o main -Wall -Wextra -std=c99 -lm
clean:
	rm -f main
pack:
	zip -FSr 312CA_ArghirMonica_Tema2.zip README Makefile *.c *.h
.PHONY: clean
