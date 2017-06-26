//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <float.h>

#define min(x,y) ((x) < (y) ? (x) : (y))
#define max(x,y) ((x) > (y) ? (x) : (y))

struct pair {
	long long start;
	long long length;
};

FILE *in, *out;

//run - subarray of input array
//run-array can have a length bigger then minrun 
//minrun - minimal size of run-subarray
inline int getMinrun(int n)
{ //getting the minimal size of run
	int r = 0;  // becomes 1 if the least significant bits contain at least one off bit 
	while (n >= 64) {
		r |= n & 1;
		n >>= 1;
	}
	return n + r;
}

inline void reverse(int *a, int lo, int hi)
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
{
	bool order;
	int j;
	if (n - i <= 2)
	{
		if (n - i == 2 && a[i] > a[i + 1])
		{
			int temp = a[i];
			a[i] = a[i + 1];
			a[i + 1] = temp;
		}
		return n;
	}
	order = a[i] > a[i + 1] ? true : false;
	j = i + 2;
	while (j < n && ((!order && a[j - 1] <= a[j]) || (order && a[j - 1] >= a[j])))
		j++;
	if (order)
		reverse(a, i, j);
	i = j;

	return i;
}

int binsearch(int *a, int start, int m, int search)
{
	int l = start;
	int r = m - 1;
	int mid;
	while (r - l > 1)
	{
		mid = l + ((r - l) >> 1);
		if (a[mid] <= search)
			l = mid;
		else
			r = mid;
	}
	if (a[l] > search)
		return l;
	else
		return r;
}

void insertionSort(int *a, int lo, int hi)
{
	int pos, x;
	for (int i = lo + 1; i < hi; i++)
	{
		if (a[i - 1] <= a[i])
			continue;
		x = a[i];
		pos = binsearch(a, lo, i, x);
		for (int j = i - 1; j >= pos; j--)
			a[j + 1] = a[j];
		a[pos] = x;
	}

}

void gallop(int *a, int beg, int end, int num, int x, bool dir, int *ans)
{
	if (!dir)
	{
		// fprintf(out, "IN asc GALLOP\n");
		if (beg + num - 1 < end && a[beg + num - 1] <= x)
			gallop(a, beg, end, min(end - beg + 1, 2 * num), x, dir, ans);
		if (*ans == -100500)
			*ans = binsearch(a, max(beg, beg + num / 2 - 1), min(end, beg + num - 1) + 1, x);

	}
	else
	{
		// fprintf(out, "IN desc GALLOP\n");
		if (end - num + 1 > beg && a[end - num + 1] >= x)
			gallop(a, beg, end, min(end - beg + 1, num * 2), x, dir, ans);
		if (*ans == -100500)
			*ans = binsearch(a, max(beg, end - num + 1), min(end, end - num / 2 + 1) + 1, x);

	}
}



void merge(int *a, int n, struct pair X, struct pair Y)
{
	int *aux;
	int auxSize = min(X.length, Y.length);
	aux = (int*)malloc(auxSize * sizeof(int));
	int cnt = 0;
	bool order = true; //copied from aux
	if (X.length < Y.length)
	{
		memcpy(aux, a + X.start, X.length * sizeof(int));
		int i = 0;
		int j = Y.start;
		int k = X.start;
		while (i < auxSize && j < Y.start + Y.length)
		{
			if (aux[i] < a[j])
			{
				a[k++] = aux[i++];
				if (order)
					cnt++;
				else
				{
					order ^= 1;
					cnt = 1;
				}
			}
			else
			{
				a[k++] = a[j++];
				if (!order)
					cnt++;
				else
				{
					order ^= 1;
					cnt = 1;
				}
			}
			if (cnt >= 7)
			{
				if (order)
				{
					int finish = -100500;
					gallop(aux, i, auxSize - 1, 1, a[j], false, &finish);
					if (finish > i)
					{
						memcpy(a + k, aux + i, (finish - i) * sizeof(int));
						k += (finish - i);
						// fprintf(out, "Straight and order:   %lli %lli ", i, finish);
						i = finish;

					}
				}
				else
				{
					int finish = -100500;
					gallop(a, j, Y.start + Y.length - 1, 1, aux[i], false, &finish);
					if (finish > j)
					{
						int *store;
						store = (int*)malloc((finish - j) * sizeof(int));
						memcpy(store, a + j,
							(finish - j) * sizeof(int));
						memcpy(a + k, store, (finish - j) * sizeof(int));
						k += (finish - j);
						// fprintf(out, "Straight and !order:  %lli %lli ", i, finish);
						j = finish;

						free(store);
					}
				}

				cnt = 0;
			}
		}
		if (i < auxSize)
		{
			for (; k < Y.start + Y.length; k++)
				a[k] = aux[i++];
		}
	}
	else
	{
		memcpy(aux, a + Y.start, Y.length * sizeof(int));
		int i = X.start + X.length - 1;
		int j = Y.length - 1;
		int k = Y.start + Y.length - 1;
		while (i >= X.start && j >= 0)
		{
			if (aux[j] > a[i])
			{
				a[k--] = aux[j--];
				if (order)
					cnt++;
				else
				{
					order ^= 1;
					cnt = 1;
				}
			}
			else
			{
				a[k--] = a[i--];
				if (!order)
					cnt++;
				else
				{
					order ^= 1;
					cnt = 1;
				}
			}
			if (cnt >= 7)
			{
				if (order)
				{
					int begin = -100500;
					gallop(aux, 0, j, 1, a[i], true, &begin);
					if (begin < j && begin >= 0)
					{
						memcpy(a + k - (j - begin), aux + begin,
							(j - begin + 1) * sizeof(int));
						k -= (j - begin + 1);
						// fprintf(out, "Reverse and order:  %lli %lli ", begin, j);
						j = begin - 1;
					}
				}
				else
				{
					int begin = -100500;
					gallop(a, X.start, i, 1, aux[j], true, &begin);
					if (begin < i && begin >= X.start)
					{
						int *store;
						store = (int*)malloc((i - begin + 1) * sizeof(int));
						memcpy(store, a + begin,
							(i - begin + 1) * sizeof(int));
						memcpy(a + k - (i - begin), store,
							(i - begin + 1) * sizeof(int));
						k -= (i - begin + 1);
						// fprintf(out, "Reverse and !order:  %lli %lli ", begin, i);
						i = begin - 1;
						free(store);
					}
				}

				cnt = 0;
			}
		}
		if (j >= 0)
		{
			for (; k >= X.start; k--)
				a[k] = aux[j--];
		}

	}



	free(aux);
}

void mergeAll(int *a, int n, struct pair *stack, int *sp)
{ //used for merging subarrays 
	int w, x, y, z;
	bool wxy, xyz, yz;
	while (true)
	{
		if (*sp <= 1)
			break;

		z = stack[*sp - 1].length;
		y = stack[*sp - 2].length;
		if (*sp == 2)
		{
			merge(a, n, stack[*sp - 2], stack[*sp - 1]);
			stack[*sp - 2].length += z;
			(*sp)--;
			break;
		}

		x = stack[*sp - 3].length;
		if (*sp >= 4)
			wxy = (stack[*sp - 4].length <= x + y) ? true : false;
		else
			wxy = false;

		xyz = (x <= y + z) ? true : false;
		xyz += wxy;
		yz = (y <= z) ? true : false;

		if (!xyz && !yz)
			break;

		if (xyz && !yz) //left merge
		{
			merge(a, n, stack[*sp - 3], stack[*sp - 2]);
			stack[*sp - 3].length += y;
			stack[*sp - 2] = stack[*sp - 1];
			(*sp)--;
		}
		else //right merge
		{
			merge(a, n, stack[*sp - 2], stack[*sp - 1]);
			stack[*sp - 2].length += x;
			(*sp)--;
		}
	}

}

bool partition(int *a, int n, int *cur, int minrun, struct pair *stack, int *sp)
{ //diving input array into sorted runs and pushing them in stack
  //sp = StackPointer - points on the start unused cell
	int j = getRun(a, *cur, n, minrun);
	if (j - *cur < minrun && j < n)
	{
		j = min(n, *cur + minrun);
		insertionSort(a, *cur, j);
	}
	stack[*sp].start = *cur;
	stack[*sp].length = j - *cur;
	(*sp)++;
	*cur = j;


	if (*cur == n)
	{
		while (*sp > 1)
		{
			merge(a, n, stack[*sp - 2], stack[*sp - 1]);
			stack[*sp - 2].length += stack[*sp - 1].length;
			(*sp)--;
		}
		free(stack);
		return true;
	}
	return false;
}

inline bool is_pyramide(int *a, struct pair *stack, int *sp)
{
	if (*sp < 2)
		return true;
	int z = stack[*sp - 1].length;
	int y = stack[*sp - 2].length;
	return (*sp == 2 && y <= z) ? false : true;
	int x = stack[*sp - 3].length;
	return ((x <= y + z) || (y <= z)) ? false : true;
}

void timsort(int *a, int n)
{
	if (n <= 1)
		return;
	if (n <= 64)
	{
		insertionSort(a, 0, n);
		return;
	}
	int minrun = getMinrun(n);
	struct pair *stack;
	int stackSize = n / minrun + 1;
	stack = (struct  pair*)malloc(stackSize * sizeof(struct pair));
	int sp = 0; //StackPointer
	int cur = 0;

	for (int i = 0; i < 3; i++)
	{
		if (partition(a, n, &cur, minrun, stack, &sp))
			return;

	}

	while (true)
	{
		if (!is_pyramide(a, stack, &sp))
		{
			mergeAll(a, n, stack, &sp);
			continue;
		}

		if (partition(a, n, &cur, minrun, stack, &sp))
			return;
	}

}

long long cmp(const void *a, const void *b)
{
   return ( *(long long*)a - *(long long*)b );
}

int main(int argc, char* argv[])
{
	
	in = fopen("in.txt", "r");
	out = fopen("out.txt", "w");
	for(long long q = 500; q < 100001; q++) {
		srand(time(NULL));
		long long n = q;
		// fscanf(in, "%lli", &n);
		long long *a, *b;
		a = (long long*)malloc(n * sizeof(long long)); 
		b = (long long*)malloc(n * sizeof(long long)); 
		// fprintf(out, "Unsorted array: ");
		for(long long i = 0; i < n; i++)
		{
			a[i] = rand() % 1000000000;
			b[i] = a[i];
			// fprintf(out, "%lli ", a[i]);
		}

		timsort(a, n);
		qsort (b, n, sizeof(long long), cmp);

		for(long long j = 0; j < n; j++)
		{
			if(b[j] != a[j])
			
			{
				fprintf(out, "\n%lli %lli\n", a[j], b[j]);
				fprintf(out, "NO\n");
				fprintf(out, "\nUnsorted array: ");
				for(long long i = 0; i < n; i++)
				{
					fprintf(out, "%lli ", a[i]);
					if(a[i] < a[i-1])
						fprintf(out, "\n%lli \n", a[i]);
				}
				fprintf(out, "\nSorted array: ");
				for(long long i = 0; i < n; i++)
				{
					fprintf(out, "%lli ", b[i]);
				}

				fclose(in);
				fclose(out);
				return 0;
			}
			// fprintf(out, "%lli ", a[i]);
		}

		
		// fprintf(out, "YES\n");
		free(a);
		free(b);
	}
	fprintf(out, "YES\n");
	fclose(in);
	fclose(out);

	return 0;
}