#include <iostream>
#include <iomanip>

void displaymat(double* p, int r, int c)
{

	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
		{
			std::cout << std::setw(15) << *(p + i * c + j) << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << "\n\n" << std::endl;
}

double determinent(double* p, int m)
{
	double innersum;
	int i, j, k;

	if (m == 1)
	{
		return *p;
	}
	else if (m == 2)
	{
		double a, b, c, d;
		a = *p;
		b = *(p + 1);
		c = *(p + 2);
		d = *(p + 3);
		return a * d - b * c;
	}
	else
	{
		innersum = 0;
		int sign;
		for (i = 0; i < m; i++)
		{
			int l = 0;
			double* q = (double*)malloc((m - 1) * (m - 1) * sizeof(double));
			for (j = 0; j < m; j++)
			{
				for (k = 0; k < m; k++)
				{
					if (j != 0 && k != i)
					{
						*(q + l) = *(p + j * m + k);
						l++;
					}
				}
			}
			if (i % 2 == 0)
			{
				sign = 1;
			}
			else
			{
				sign = -1;
			}
			innersum = innersum + *(p + i) * determinent(q, m - 1) * sign;
			free(q);
		}
	}
	return innersum;
}

void analytic_solution(double* p, double* inv, int d, double det)
{
	int i, j, m, n, l, sign;

	for (i = 0; i < d; i++)
	{
		for (j = 0; j < d; j++)
		{
			l = 0;
			double* q = new double[(d - 1) * (d - 1)];
			for (m = 0; m < d; m++)
			{
				for (n = 0; n < d; n++)
				{
					if (m != i && n != j)
					{
						*(q + l) = *(p + m * d + n);
						l++;
					}
				}
			}
			if ((i + j) % 2 == 0)
			{
				sign = 1;
			}
			else
			{
				sign = -1;
			}
			*(inv + j * d + i) = determinent(q, d - 1) * sign / det;
			delete[] q;
		}
	}
}

int main()
{
	int dim;
	std::cout << "Enter Matrix dimention(Has to be a square matrix): ";
	std::cin >> dim;
	std::cout << std::endl;
	double* mat = new double[dim * dim];
	double* invmat = new double[dim * dim];

	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			*(invmat + i * dim + j) = 0;
		}
	}

	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			std::cout << "a[" << i << "][" << j << "]:";
			std::cin >> *(mat + i * dim + j);
		}
		std::cout << std::endl;
	}

	std::cout << "\n" << std::endl;

	double det = determinent((double*)mat, dim);
	analytic_solution((double*)mat, (double*)invmat, dim, det);

	system("cls");
	std::cout << "INVERSE MATRIX:" << "\n" << std::endl;
	displaymat((double*)invmat, dim, dim);

	delete[] mat;
	delete[] invmat;

}
