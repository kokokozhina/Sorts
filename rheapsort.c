#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct tree tree;

struct tree {
	int val;
	tree *l;
	tree *r;
};

void swap(tree **l, tree **r)
{
	tree *temp = *l;
	*l = *r;
	*r = temp;
}

tree *merge(tree *tl, tree *tr)
{
	if (!tl || !tr)
		return tl ? tl : tr;
	if (tr->val < tl->val)
		swap(&tl, &tr);
	if (rand() & 1)
		swap(&tl->l, &tl->r);
	tl->l = merge(tl->l, tr);
	return tl;
}

tree *add(tree *t, int node)
{
	tree *aux;
	aux = (tree*)malloc(sizeof(tree));
	aux->val = node;
	aux->l = NULL;
	aux->r = NULL;
	if (t)
		t = merge(t, aux);
	else
		t = aux;
	return t;
}

void rheap_sort(tree *t, int *a, int n)
{
	tree *t = NULL;
	for (int i = 0; i < n; i++)
	{
		t = add(t, a[i]);
	}
	tree *templ, *tempr;
	for (int i = 0; i < n; i++)
	{
		a[i] = t->val;
		templ = t->l;
		tempr = t->r;
		free(t);
		t = merge(templ, tempr);
	}
}

int main(int argc, char* argv[])
{

	FILE *in, *out;
	in = fopen("in.txt", "r");
	out = fopen("out.txt", "w");

	srand(time(NULL));

	for(int q = 5000; q <= 100000; q+=5000)
	{
		int n = q;
		int *a;
		a = (int*)malloc(n * sizeof(int));
		unsigned long long start_t, end_t, total_t = 0;
		int iter = 1000;
		for (int z = 0; z < iter; z++)
		{
			
			for (int i = 0; i < n; i++)
			{
				a[i] = rand() % 1000000;
				//fprintf(out, "%d ", a[i]);
			}
			start_t = clock();
			rheap_sort(a, n);
			end_t = clock();
			total_t += end_t - start_t;
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

				fprintf(out, "\nSorted array:   ");

				for (int i = 0; i < n; i++)
					fprintf(out, "%d ", a[i]);
				 fclose(in);
				 fclose(out);
				 return 0;
			}
			//free(a);
		}
		free(a);
		fprintf(out, "(%d, %f)\n", n / 1000, total_t / (double)iter / CLOCKS_PER_SEC);
	}
	fclose(in);
	fclose(out);
	return 0;
}