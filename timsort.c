#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
//using https://neerc.ifmo.ru/wiki/index.php?title=Timsort
void swap(int *a, int i, int j) 
{
	int temp = a[i];
	a[i] = a[j];
	a[j] = temp;
}


//run - подмассив во входном массиве (упорядоченный как-либо)
//minrun - минимальный размер подмассива run
int getMinrun(int n)
{
    int r = 0;  /* becomes 1 if the least significant bits contain at least one off bit */
    while (n >= 64) {
        r |= n & 1;
        n >>= 1;
    }
    return n + r;
}

void reverse(int *a, int lo, int hi)
{
	int upper_bound = lo + (hi - lo) / 2;
	for(int i = lo, j = hi - 1; i < upper_bound; i++, j--)
		swap(a, i, j);
}

int getRun(int *a, int i, int n, int minrun)
{
	int j = i + 1;
	bool order = true; //ascending = 1, descending = 0;
	while(j < n && j - i < minrun) 
	{
		if(n - j >= 2)
		{
			j++;
			order = a[i] <= a[j - 1] ? true : false;
			while(j < n && (order && a[i] <= a[j] || !order  && a[i] >= a[j]))
				j++;
			if(!order)
				reverse(a, i, j);
		}
		else 
			j++;
	}
	return j;
}

void insertionSort(int *a, int n, int lo, int hi)
{
	for (int i = lo; i < hi - 1; i++)
	{
		for (int j = i + 1; j < hi; j++)
		{
			if (a[i] > a[j])
				swap(a, i, j);
		}
	}
}

int partition(int *a, int n) 
{
	int minrun = getMinrun(n);
	int i = 0, j = 1;
	while(i < n) 
	{
		j = getRun(a, i, n, minrun);
		insertionSort(a, n, i, j);
		i = j;
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
		a[i] = rand() % 1000;
		fprintf(out, "%d ", a[i]);
	}
	//timsort(a, n);
	partition(a, n);
	fprintf(out, "\nminrun:   %d", getMinrun(n));
	fprintf(out, "\nSorted array:   ");
	for(int i = 0; i < n; i++)
		fprintf(out, "%d ", a[i]);
	

	fclose(in);
	fclose(out);
	return 0;
}
