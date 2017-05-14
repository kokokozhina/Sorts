// #define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
//using https://neerc.ifmo.ru/wiki/index.php?title=Timsort

FILE *in, *out;

struct pair {
	int first;
	int second;
};

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
	for (int i = lo, j = hi - 1; i < upper_bound; i++, j--)
		swap(a, i, j);
}

int getRun(int *a, int i, int n, int minrun)
{
	int j = i + 1;
	bool order = true; //ascending = 1, descending = 0;
	while (j < n && j - i < minrun)
	{
		if (n - j >= 2)
		{
			j++;
			order = a[i] <= a[j - 1] ? true : false;
			while (j < n && (order && a[j - 1] <= a[j] || !order  && a[j - 1] >= a[j]))
				j++;
			if (!order)
				reverse(a, i, j);
		}
		else
			j++;
	}
	return j;
}

void insertionSort(int *a, int n, int lo, int hi)//Kormen, 2013, page 48
{
	for (int j = lo + 1; j < hi; j++)
	{
		int key = a[j];
		int i = j - 1;
		while (i >= lo && a[i] > key)
		{
			a[i + 1] = a[i];
			i--;
		}
		a[i + 1] = key;
	}
}

void partition(int *a, int n, int minrun, struct pair *stack, int *sp)
{

	int i = 0, j = 1;
	struct pair subarray;
	while (i < n)
	{
		j = getRun(a, i, n, minrun);
		insertionSort(a, n, i, j);
		subarray.first = i;
		subarray.second = j - i;
		// fprintf(out, "%d \n", *sp);
		(*sp)++;
		stack[*sp - 1] = subarray;//так как sp указывает на первую неиспользованную ячейку
		i = j;
	}
	// fprintf(out, "%d \n", *sp);
}

void merge(int *a, int n, struct pair X, struct pair Y)
{
	int *aux;
	int auxSize = X.second;
	aux = (int*)malloc(auxSize * sizeof(int));
	int i = 0, j = X.first;
	for (int i = 0; i < auxSize; i++)
	{
		aux[i] = a[j];
		j++;
	}
	// memcpy(aux, *a + X.first, X.second);
	i = 0; //указатель на первый элемент aux
	j = 0;
	int k = X.first;
	while (i < X.second && j < Y.second)
	{
		if (aux[i] <= a[j + Y.first])
		{
			a[k] = aux[i];
			i++;
		}
		else
		{
			a[k] = a[j + Y.first];
			j++;
		}
		k++;
	}

	for (; i < auxSize; i++)
	{
		a[k] = aux[i];
		k++;
	}
}

void mergeAll(int *a, int n, struct pair *stack, int stackSize, int *sp)
{
	int x, y, z;
	while (*sp > 3) //>=???
	{
		x = stack[*sp - 1].second; 
		y = stack[*sp - 2].second; 
		z = stack[*sp - 3].second; 
		if (z <= x + y)
		{
			if (x < z)
			{
				merge(a, n, stack[*sp - 2], stack[*sp - 1]);
				stack[*sp - 2].second += x;
				(*sp)--;
			}
			else
			{
				merge(a, n, stack[*sp - 3], stack[*sp - 2]);
				stack[*sp - 3].second += y;
				stack[*sp - 2] = stack[*sp - 1];
				(*sp)--;
			}
		}
		else //if(y <= x)//????
		{
			merge(a, n, stack[*sp - 2], stack[*sp - 1]);
			stack[*sp - 2].second += x;
			(*sp)--;
		}
	}
	if (*sp == 3)
	{
		x = stack[*sp - 1].second;
		y = stack[*sp - 2].second;
		merge(a, n, stack[*sp - 2], stack[*sp - 1]);
		stack[*sp - 2].second += x;
		(*sp)--;
	}
	if (*sp == 2)
	{
		x = stack[*sp - 1].second;
		y = stack[*sp - 2].second;
		merge(a, n, stack[*sp - 2], stack[*sp - 1]);
		stack[*sp - 2].second += x;
		(*sp)--;
	}
}

void timsort(int *a, int n)
{
	int minrun = getMinrun(n);
	struct pair *stack;
	int stackSize = n / minrun + 1;
	stack = (struct  pair*)malloc(stackSize * sizeof(struct pair));
	int sp = 0; //sp = stackPointer
	partition(a, n, minrun, stack, &sp);
	// for (int i = 0; i < sp; i++)
	// {
	// 	fprintf(out, "\n%d %d\n", stack[i].first, stack[i].second);
	// }
	mergeAll(a, n, stack, stackSize, &sp);
	// merge(a, n, stack[sp - 2], stack[sp - 1]);
	// (*sp)--;
}

int main(int argc, char* argv[])
{

	in = fopen("in.txt", "r");
	out = fopen("out.txt", "w");

	srand(time(NULL));
	int n;
	//fscanf(in, "%d", &n);
	
	for(int z = 0; z < 10000000; z++)
	{
		//fprintf(out, "Unsorted array: ");
		n = rand() % 1000 + 1;
		int *a;
		a = (int*)malloc(n * sizeof(int));
		for (int i = 0; i < n; i++)
		{
			a[i] = rand() % 1000000;
			//fprintf(out, "%d ", a[i]);
		}
		timsort(a, n);
		bool flag = false;

		for (int i = 1; i < n; i++)
		{
			if (a[i] < a[i - 1])
			{
				fprintf(out, "\nNO %d\n ", i);
				flag = true;
				break;
			}
		}
		if(flag)
		{
			fprintf(out, "\nn:   %d", n);

			fprintf(out, "\nminrun:   %d", getMinrun(n));
			fprintf(out, "\nSorted array:   ");
		
			for (int i = 0; i < n; i++)
				fprintf(out, "%d ", a[i]);	
			fclose(in);
			fclose(out);
			return 0;
		}
	}
	fprintf(out, "YES\n ");
	fclose(in);
	fclose(out);
	return 0;
}