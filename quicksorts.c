#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>

void swap(int *a, int i, int j) 
{
	int temp = a[i];
	a[i] = a[j];
	a[j] = temp;
}

void quicksort(int *a, int l, int r) //N.Virt 
{
	int i, j, x, w;

	i = l;
	j = r;
	x = a[(l + r) / 2]; //pivot is the middle of array
	do
	{
		while((i < r) && (a[i] < x))
			i++;
		while((j > l) && (x < a[j]))
			j--;
		if(i <= j)
		{
			swap(a, i, j); 
			i++;
			j--;
		}
	} while(i < j);

	if(l < j) quicksort(a, l, j);
	if(i < r) quicksort(a, i, r);
}

//2-pivots
void dualPivotQuicksort(int *a, int left, int right, int div) 
{
	int len = right - left;
	if (len < 27) 
	{ // insertion sort for tiny array
		for (int i = left + 1; i <= right; i++) 
			for (int j = i; j > left && a[j] < a[j - 1]; j--) 
				swap(a, j, j - 1);
		return;
	}

	int third = len / div;

	// "medians"
	int m1 = left + third;
	int m2 = right - third;

	if (m1 <= left) 
		m1 = left + 1;
	
	if (m2 >= right) 
		m2 = right - 1;
	
	if (a[m1] < a[m2]) 
	{
		swap(a, m1, left);
		swap(a, m2, right);
	}
	else 
	{
		swap(a, m1, right);
		swap(a, m2, left);
	}
	// pivots
	int pivot1 = a[left];
	int pivot2 = a[right];
	// pointers
	int less = left + 1;
	int great = right - 1;
	// sorting
	for (int k = less; k <= great; k++) 
	{
		if (a[k] < pivot1) 
			swap(a, k, less++);
		else if (a[k] > pivot2) 
		{
			while (k < great && a[great] > pivot2) 
				great--;
			
			swap(a, k, great--);
			if (a[k] < pivot1) 
				swap(a, k, less++);
		}
	}
	// swaps
	int dist = great - less;
	if (dist < 13) 
		div++;

	swap(a, less - 1, left);
	swap(a, great + 1, right);
	// subarrays
	dualPivotQuicksort(a, left, less - 2, div);
	dualPivotQuicksort(a, great + 2, right, div);
	
	// equal elements
	if (dist > len - 13 && pivot1 != pivot2) 
		for (int k = less; k <= great; k++) 
			if (a[k] == pivot1) 
				swap(a, k, less++);
			else if (a[k] == pivot2) 
			{
				swap(a, k, great--);
				if (a[k] == pivot1) 
					swap(a, k, less++);
			}
		
	// subarray
	if (pivot1 < pivot2)
		dualPivotQuicksort(a, less, great, div);
}

//3 med
void quicksortMedOf3(int *a, int l, int r) //Kormen
{
	int i, j, x, w;

	i = l;
	j = r;

	if(r - l + 1 >= 3) 
	{
		int b[3]; //choosing 3 different random elements
		b[0] = rand() % (r - l + 1) + l;
		b[1] = rand() % (r - l + 1) + l;
		b[2] = rand() % (r - l + 1) + l;
		while(b[1] == b[0])
			b[1] = rand() % (r - l + 1) + l;
		while(b[2] == b[0] || b[2] == b[1])
			b[2] = rand() % (r - l + 1) + l;
		if(a[b[0]] > a[b[1]]) swap(b, 0, 1); //getting the medium
		if(a[b[0]] > a[b[2]]) swap(b, 0, 2);
		if(a[b[1]] > a[b[2]]) swap(b, 1, 2);
		x = a[b[1]];  
	}
	else 
		x = a[(l + r) / 2];
	
	do
	{
		while(i < r && a[i] < x)
			i++;
		while(j > l && x < a[j])
			j--;
		if(i <= j)
		{
			swap(a, i, j); 
			i++;
			j--;
		}
	} while(i < j);

	if(l < j) quicksortMedOf3(a, l, j);
	if(i < r) quicksortMedOf3(a, i, r);
}


int main (int argc, char* argv[]) 
{
	FILE *in, *out; 
	in = fopen("in.txt", "r");
	out = fopen("out.txt", "w");

	srand(time(NULL));
	int n, type;
	fscanf(in, "%d%d", &n, &type);
	int *a;
	a = (int*)malloc(n * sizeof(int)); 

	fprintf(out, "Unsorted array: ");
	for(int i = 0; i < n; i++)
	{
		a[i] = rand() % 100;
		fprintf(out, "%d ", a[i]);
	}

	if(type == 1)
		quicksort(a, 0, n - 1);
	else if(type == 2)
		dualPivotQuicksort(a, 0, n - 1, 3);
	else if(type == 3)
		quicksortMedOf3(a, 0, n - 1);

	fprintf(out, "\nSorted array:   ");
	for(int i = 0; i < n; i++)
		fprintf(out, "%d ", a[i]);
	

	fclose(in);
	fclose(out);
	return 0;
}