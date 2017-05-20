#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>

int heap_size;

void swap(int *a, int i, int j) 
{
	int temp = a[i];
	a[i] = a[j];
	a[j] = temp;
}

int inline parent(int i)
{
	return ((i - 1) >> 1);
}

int inline left(int i)
{
	return (i << 1) + 1;
}

int inline right(int i)
{
	return (i << 1) + 2;
}

void max_heapify(int *a, int i)
{
	int l = left(i);
	int r = right(i);
	int largest = i;
	if(l < heap_size && a[l] > a[i])
		largest = l;
	if(r < heap_size && a[r] > a[largest]) 
		largest = r;
	if(largest != i)
	{
		swap(a, i, largest);
		max_heapify(a, largest);
	}
}

void build_max_heap(int *a, int n)
{
	heap_size = n;
	for(int i = n / 2; i >= 0; i--)
		max_heapify(a, i);
}

void heap_sort(int *a, int n) //Kormen
{
	build_max_heap(a, n);
	for(int i = n - 1; i > 0; i--)
	{
		swap(a, 0, i);
		heap_size--;
		max_heapify(a, 0);
	}
}

void heap_sort_mod(int *a, int n) //Kormen
{
	int i;

	for (i = 1; i < n; ++i)
	{
       int r = i;
       int x = a[r];
       for (;;)
       {
       	  int l = parent (r);
       	  if (a[l] >= x)
       	  	break;
       	  a[r] = a[l];
       	  r = l;
       }
       a[r] = x;
	}

	for(int i = n - 1; i > 0; i--)
	{
		swap(a, 0, i);
		heap_size--;
		max_heapify(a, 0);
	}
}


int main (int argc, char* argv[]) 
{
	FILE *in, *out; 
	in = fopen("in.txt", "r");
	out = fopen("out.txt", "w");

	srand(time(NULL));
	int n;
	fscanf(in, "%d", &n);
	int *a;
	a = (int*)malloc(n * sizeof(int)); 

	fprintf(out, "Unsorted array: ");
	for(int i = 0; i < n; i++)
	{
		a[i] = rand() % 100;
		fprintf(out, "%d ", a[i]);
	}

	heap_sort(a, n);

	fprintf(out, "\nSorted array:   ");
	for(int i = 0; i < n; i++)
		fprintf(out, "%d ", a[i]);
	
	free(a);
	fclose(in);
	fclose(out);
	return 0;
}