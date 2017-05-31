#include <float.h>

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
	int iter = 500;
	double *x, *y;
	x = (double*)malloc(20 * sizeof(double)); //20 = 5, 10, 15, ..., 100
	y = (double*)malloc(20 * sizeof(double));
	int rep_ind = 0;
	int *dispersia;
	dispersia = (int*)malloc(500 * sizeof(int));
	for (int i = 0; i < 500; i++)
	{
		dispersia[i] = 0;
	}
	for (int q = 5000; q <= 100000; q += 5000)
	{
		int n = q;
		int *a;
		a = (int*)malloc(n * sizeof(int));
		unsigned long long start_t = 0, end_t = 0, total_t = 0;
		double min_t = DBL_MAX;
		double max_t = DBL_MIN;
		for (int z = 0; z < iter; z++)
		{
			for (int i = 0; i < n; i++)
			{
				a[i] = rand() % 10000000;
				//fprintf(out, "%d ", a[i]);
			}
			start_t = clock();
			NAME_OF_SORTING_ALGORITHM(a, n); //!!
			end_t = clock();
			if (end_t < start_t)
			{
				fprintf(out, "\nTIME LIMIT  ");
				return 0;
			}
			total_t += end_t - start_t;
			bool flag = false;
			if (end_t - start_t < min_t) min_t = end_t - start_t;
			if (end_t - start_t > max_t) max_t = end_t - start_t;

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
			if (q == 100000)
			{
				int ind = (end_t - start_t) / (double)CLOCKS_PER_SEC * 1000;
				fprintf(out, "IND %d\n", ind);
				if (ind >= 0 && ind < 500)
				{
					dispersia[ind]++;
				}
			}
		}
		free(a);
		x[rep_ind] = q;
		y[rep_ind] = total_t / (double)iter / CLOCKS_PER_SEC;
		fprintf(out, "(%d, %f) %f %f\n", n / 1000, total_t / (double)iter / CLOCKS_PER_SEC, min_t / CLOCKS_PER_SEC, max_t / CLOCKS_PER_SEC);
		rep_ind++;
	}
	fprintf(out, "C=%f\n", nmk(x, y, 20));

	fprintf(out, "Dispersion:\n");
	for (int i = 0; i < 500; i++)
	{
		fprintf(out, "(%f, %d)\n", 0.0005 + i * 0.001, dispersia[i]);
	}
	fclose(in);
	fclose(out);
	return 0;
}