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

void rheap_sort(int *a, int n)
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

	rheap_sort(a, n);

	fprintf(out, "\nSorted array:   ");
	for(int i = 0; i < n; i++)
		fprintf(out, "%d ", a[i]);
	
	free(a);
	fclose(in);
	fclose(out);
	return 0;
}