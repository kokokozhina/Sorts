CC=gcc

CFLAGS=-c

all: make-quick make-heap make-rheap make-tim

make-quick: quicksorts.o
	$(CC) quicksorts.o -o quicksorts

make-heap: heapsort.o
	$(CC) heapsort.o -o heapsort

make-rheap: rheapsort.o
	$(CC) rheapsort.o -o rheapsort

make-tim: timsort.o
	$(CC) timsort.o -o timsort

quicksorts.o: quicksorts.c
	$(CC) $(CFLAGS) quicksorts.c

heapsort.o: heapsort.c
	$(CC) $(CFLAGS) heapsort.c

rheapsort.o: rheapsort.c
	$(CC) $(CFLAGS) rheapsort.c

timsort.o: timsort.c
	$(CC) $(CFLAGS) timsort.c

clean:
	rm -rf *.o all