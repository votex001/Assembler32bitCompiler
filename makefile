
FLAGS = -ansi -Wall -pedantic
OTHER = global.h
OBJECTS = assembler.o utils.o preprocess.o


assembler: ${OBJECTS} ${OTHER}
	gcc $(OBJECTS) ${FLAGS} -o $@

utils.o: utils.c utils.h ${OTHER}
	gcc -c utils.c ${FLAGS} -o $@

preprocess.o: preprocess.c preprocess.h ${OTHER}
	gcc -c preprocess.c ${FLAGS} -o $@
