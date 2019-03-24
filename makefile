all: ejemplo1.c ejemplo2.c ejemplo3
	gcc -o ejemplo1 ejemplo1.c -pthread
	gcc -o ejemplo2 ejemplo2.c
	gcc -o ejemplo3 ejemplo3.c

ejemplo1: ejemplo1.c
	gcc -o ejemplo1 ejemplo1.c -pthread
ejemplo2: ejemplo2.c
	gcc -o ejemplo2 ejemplo2.c
ejemplo3: ejemplo3.c
	gcc -o ejemplo3 ejemplo3.c 

clean: 
	rm ejemplo1
	rm ejemplo2
	rm ejemplo3