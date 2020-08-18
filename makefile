assembler : assembler.o   first_pass.o memory_map.o auxiliary.o assembler.h definition.h  first_pass.h memory_map.h auxiliary.h
	gcc -g -Wall -ansi -pedantic assembler.o first_pass.o memory_map.o -o assembler

assembler.o : assembler.c assembler.h auxiliary.h first_pass.h memory_map.h definition.h
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o
memory_map.o: memory_map.c memory_map.h definition.h
	gcc -c -Wall -ansi -pedantic memory_map.c -o memory_map.o 
first_pass.o : first_pass.c  first_pass.h  memory_map.c memory_map.h definition.h
	gcc -c -Wall -ansi -pedantic first_pass.c -o first_pass.o
auxiliary.o :auxiliary.c auxiliary.h first_pass.h
	gcc -c -Wall -ansi -pedantic auxiliary.c -o auxiliary.o

