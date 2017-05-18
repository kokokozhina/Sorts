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
	int n;

	for (int z = 0; z < 10000; z++)
	{
		n = rand() % 100000;
		if (n == 0)
			continue;
		int *a;
		tree *t = NULL;
		a = (int*)malloc(n * sizeof(int));
		for (int i = 0; i < n; i++)
		{
			a[i] = rand() % 1000000;
			t = add(t, a[i]);
		}
		rheap_sort(t, a, n);
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

		free(a);
	}
	fprintf(out, "YES\n ");
	fclose(in);
	fclose(out);
	return 0;
}