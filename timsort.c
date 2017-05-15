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

void swap(int *a, int i, int j)
{
	int temp = a[i];
	a[i] = a[j];
	a[j] = temp;
}


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
	for (int i = lo, j = hi - 1; i < upper_bound; i++, j--)
		swap(a, i, j);
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

int binsearch(int *aux, int m, int start, int search)
{
	int l = start;
	int r = m - 1;
	int mid;
	while (r - l > 1)
	{
		mid = l + (r - l) / 2;
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
	bool order = false; //the last сopied element was from X-subarray = false, Y = true;
	int cnt = 0;
	while (i < X.second && j < Y.second)
	{
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
				finish = binsearch(a, Y.first + Y.second, j + Y.first, aux[i]); //upper_bound
				cnt += finish - j;
				if (j + Y.first != k)
				{
					for (int z = j + Y.first; z < finish; z++, k++, j++)
					{
						a[k] = a[j + Y.first];
					}
				}
				
			}
			else
			{
				finish = binsearch(aux, auxSize, i, a[j + Y.first]);
				cnt += finish - i;
				for (int z = i; z < finish; z++, k++, i++)
				{
					a[k] = aux[i];
				}
			}
		}
	}
	for (; i < auxSize; i++)
	{
		a[k] = aux[i];
		k++;
	} //we can free ourselves from copying a tail of Y subarray - it's already in his place
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
	for(int i = *sp; i > 1; i--)
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

int main(int argc, char* argv[])
{
	FILE *in, *out;
	in = fopen("in.txt", "r");
	out = fopen("out.txt", "w");

	srand(time(NULL));
	int n;

	for (int z = 0; z < 100000; z++)
	{
		//fprintf(out, "Unsorted array: ");
		n = rand() % 100000;
		if(n == 0)
			continue;
		int *a;
		a = (int*)malloc(n * sizeof(int));
		for (int i = 0; i < n; i++)
		{
			a[i] = rand() % 1000000;
			fprintf(out, "%d ", a[i]);
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
		if (flag)
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
		free(a);
	}
	fprintf(out, "YES\n ");
	fclose(in);
	fclose(out);
	return 0;
}