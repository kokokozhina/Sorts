// #define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
//using https://neerc.ifmo.ru/wiki/index.php?title=Timsort
struct pair {
	int first;
	int second;
};

//run - subarray of input array
//run-array can have a length bigger then minrun 
//minrun - minimal size of run-subarray
int getMinrun(int n)
{ //getting the minimal size of run
	int r = 0;  // becomes 1 if the least significant bits contain at least one off bit 
	while (n >= 64) {
		r |= n & 1;
		n >>= 1;
	}
	return n + r;
}

void reverse(int *a, int lo, int hi)
{ //reversing a array in [lo, hi)
	int upper_bound = lo + (hi - lo) / 2;
	int temp;
	for (int i = lo, j = hi - 1; i < upper_bound; i++, j--)
	{ //swap(a, i, j);
		temp = a[i];
		a[i] = a[j];
		a[j] = temp;
	}
}

int getRun(int *a, int i, int n, int minrun)
{ //returns the end of subarray (sorted in ascending order) with length >= minrun and beginning at i
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

void insertionSort(int *a, int n, int lo, int hi) //Kormen, 2013, page 48
{ //used for sorting runs (subarray with length >= minrun)
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
{ //diving input array into sorted runs and pushing them in stack
  //sp = StackPointer - points on the first unused cell
	int i = 0, j = 1;
	struct pair subarray;
	while (i < n)
	{
		j = getRun(a, i, n, minrun);
		insertionSort(a, n, i, j);
		subarray.first = i;
		subarray.second = j - i;
		(*sp)++;
		stack[*sp - 1] = subarray;
		i = j;
	}
}

int binsearch(int *aux, int start, int m, int search)
{
	int l = start;
	int r = m - 1;
	int mid;
	while (r - l > 1)
	{
		mid = l + ((r - l) >> 1);
		if (aux[mid] <= search)
			l = mid;
		else
			r = mid;
	}
	if (aux[l] > search)
		return l;
	else
		return r;
}

void merge(int *a, int n, struct pair X, struct pair Y)
{
	int *aux;
	int auxSize = X.second;
	aux = (int*)malloc(auxSize * sizeof(int));
	int j = X.first;
	for (int i = 0; i < auxSize; i++)
	{ //copying the left subarray in order not to split the task into two separate ones: mergeLeft and mergeRight
		aux[i] = a[j];
		j++;
	}

	int i = 0;
	j = 0;
	int k = X.first;
	bool order; //the last сopied element was from X-subarray = false, Y = true;
	int cnt;
	while (i < X.second && j < Y.second)
	{
		cnt = 0;
		order = false;
		if (aux[i] <= a[j + Y.first])
		{
			a[k] = aux[i];
			i++;
			if (!order) cnt++;
			else
			{
				order ^= 1;
				cnt = 1;
			}
		}
		else
		{
			a[k] = a[j + Y.first];
			j++;
			if (order) cnt++;
			else
			{
				order ^= 1;
				cnt = 1;
			}
		}
		k++;
		if (cnt >= 7) //galloping mode
		{ //if cnt numbers was copied from array, it's assumed that some more numbers will be copied from it
			int finish;
			if (order)
			{
				finish = binsearch(a, j + Y.first, Y.first + Y.second, aux[i]); //upper_bound
				if(finish > j + Y.first)
					memmove(a + k, a + j, (finish - j - Y.first) * sizeof(int));
			}
			else
			{
				finish = binsearch(aux, i, auxSize, a[j + Y.first]);
				if(finish > i)
					memmove(a + k, aux + i, (finish - i) * sizeof(int));
			}
		}
	}
	if(auxSize > i) //we can free ourselves from copying a tail of Y subarray - it's already in his place
		memmove(a + k, aux + i, (auxSize - i) * sizeof(int));

	free(aux);
}

void mergeAll(int *a, int n, struct pair *stack, int stackSize, int *sp)
{ //used for merging subarrays 
	int x, y, z, f;
	while (*sp > 3)
	{
		x = stack[*sp - 1].second;
		y = stack[*sp - 2].second;
		z = stack[*sp - 3].second;
		if (*sp >= 4)
			f = stack[*sp - 4].second;
		else
			f = 1000000000;
		if ((z <= x + y || f <= z + y) && y <= z) //left merge
		{
			merge(a, n, stack[*sp - 3], stack[*sp - 2]);
			stack[*sp - 3].second += y;
			stack[*sp - 2] = stack[*sp - 1];
			(*sp)--;
		}
		else //right merge
		{
			merge(a, n, stack[*sp - 2], stack[*sp - 1]);
			stack[*sp - 2].second += x;
			(*sp)--;
		}
	}
	for (int i = *sp; i > 1; i--)
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
	int stackSize = n / minrun + 1; //minrun > 0 which guaranteed by if(n == 0) continue;
	stack = (struct  pair*)malloc(stackSize * sizeof(struct pair));
	int sp = 0; //sp = stackPointer
	partition(a, n, minrun, stack, &sp);
	mergeAll(a, n, stack, stackSize, &sp);
	free(stack);
}

double nmk(double *x, double *y, int n)
{
	double sumx = 0, sumy = 0;
	for (int i = 0; i < n; i++)
	{
		sumx += (x[i] / 1000) * log((x[i] / 1000));
		sumy += y[i];
	}
	return (sumy / sumx);
}

int main(int argc, char* argv[])
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

	timsort(a, n);


	fprintf(out, "\nSorted array:   ");
	for(int i = 0; i < n; i++)
		fprintf(out, "%d ", a[i]);

	
	
	free(a);
	fclose(in);
	fclose(out);
	return 0;
}