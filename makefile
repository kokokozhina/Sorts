CC=gcc

CFLAGS=-O2 -Wall

all: quicksorts heapsort heapsort_mod timsort

clean:
	rm -rf *.o 