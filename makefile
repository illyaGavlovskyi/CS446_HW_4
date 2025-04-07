make: mymalloc.o
	gcc -o mymalloc -pthread -Wall mymalloc.c
clean: 
	rm *o mymalloc