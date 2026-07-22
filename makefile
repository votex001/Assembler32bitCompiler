
FLAGS = -ansi -Wall -pedantic
OTHER = global.h
OBJECTS = assembler.o utils.o preprocess.o macro_table.o \
          file_utils.o code.o process_files.o first_pass.o \
          process_tables.o original_file_table.o second_pass.o


assembler: ${OBJECTS} ${OTHER}
	gcc $(OBJECTS) ${FLAGS} -o $@

utils.o: utils.c utils.h ${OTHER}
	gcc -c utils.c ${FLAGS} -o $@

preprocess.o: preprocess.c preprocess.h ${OTHER}
	gcc -c preprocess.c ${FLAGS} -o $@


file_utils.o: file_utils.c file_utils.h ${OTHER}
	gcc -c file_utils.c ${FLAGS} -o $@

code.o: code.c code.h ${OTHER}
	gcc -c code.c ${FLAGS} -o $@

macro_table.o: macro_table.c macro_table.h ${OTHER}
	gcc -c macro_table.c ${FLAGS} -o $@

process_files.o: process_files.c process_files.h ${OTHER}
	gcc -c process_files.c ${FLAGS} -o $@

first_pass.o: first_pass.c first_pass.h ${OTHER}
	gcc -c first_pass.c ${FLAGS} -o $@

	
process_tables.o: process_tables.c process_tables.h ${OTHER}
	gcc -c process_tables.c ${FLAGS} -o $@

original_file_table.o: original_file_table.c original_file_table.h ${OTHER}
	gcc -c original_file_table.c ${FLAGS} -o $@

second_pass.o: second_pass.c second_pass.h ${OTHER}
	gcc -c second_pass.c ${FLAGS} -o $@