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

void max_heapify(int *a, int i)
{
	int l = (i << 1) + 1;
	int r = (i << 1) + 2;
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

void heap_sort_mod(int *a, int n)
{
	for (int i = 1; i < n; ++i)
	{
		int s = i;
		int x = a[s];
		for (;;)
		{
			int p = ((s - 1) >> 1);
			if (a[p] >= x)
				break;
			a[s] = a[p];
			s = p;
		}
		a[s] = x;
	}

	heap_size = n;	
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

	heap_sort_mod(a, n);

	fprintf(out, "\nSorted array:   ");
	for(int i = 0; i < n; i++)
		fprintf(out, "%d ", a[i]);
	
	free(a);
	fclose(in);
	fclose(out);
	return 0;
}