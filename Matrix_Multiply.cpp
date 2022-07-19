#include <iostream>
#include <iomanip>
#include <cstdlib>

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

void matrix_multiply(double** a, double* b, int* rf, int* cf, int num)
{
	double sum;
	double** interim_result = new double* [num - 1]{nullptr};
	for (int i = 0; i < num - 1; i++)
	{
		interim_result[i] = new double[rf[0] * cf[i + 1]]{ 0 };
	}

	for (int i = 0; i < num - 1; i++)
	{
		for (int j = 0; j < rf[0]; j++)
		{
			for (int k = 0; k < cf[i + 1]; k++)
			{
				sum = 0;
				for (int l = 0; l < cf[i]; l++)
				{
					if (i == 0)
					{
						sum = sum + a[i][cf[i] * j + l] * a[i + 1][cf[i + 1] * l + k];
					}
					else
					{
						sum = sum + interim_result[i - 1][cf[i] * j + l] * a[i + 1][cf[i + 1] * l + k];
					}
				}
				interim_result[i][j * cf[i + 1] + k] = sum;
			}
		}
		//displaymat(interim_result[i], rf[0], cf[i + 1]);
	}
	for (int i = 0; i < rf[0] * cf[num - 1]; i++)
	{
		b[i] = interim_result[num - 2][i];
	}
	for (int i = 0; i < num - 1; i++)
	{
		delete[] interim_result[i];
	}
	delete[] interim_result;
}

int main(void)
{
	int num; // total number of matrices
	char ch = 'a';
	std::cout << "How many matrices/matrix?:";
	std::cin >> num;
	std::cout << std::endl;
	double** mat = new double* [num]; // double pointer for stacks of matrices
	int* r = new int[num]{0}; // to store the rows the matrices
	int* c = new int[num]{0}; // to store the columns of matrices
	for (int n = 0; n < num; n++)
	{
		std::cout << "matrix:" << n + 1 << "\n" << std::endl;
		std::cout << "rows:";
		std::cin >> r[n]; // input of row
		std::cout << "columns:";
		std::cin >> c[n]; // input of column
		std::cout << std::endl;
		mat[n] = new double[(r[n] * c[n])]; // for getting elements
		for (int i = 0; i < c[n] * r[n]; i++)
		{
			std::cout << ch << "[" << i / c[n] + 1 << "][" << i % c[n] + 1 << "]:";//ch << "[" << i / c[n] << "]" << "[" << i % c[n] << "]:";
			std::cin >> *(*(mat + n) + i);
		}
		displaymat(mat[n], r[n], c[n]);
		ch++;
	}

	for (int i = 0; i < num - 1; i++)
	{
		if (c[i] != r[i + 1])
		{
			std::cout << "matrix dimension needs to be matched" << std::endl;
			for (int j = 0; j < num; j++)
			{
				delete[] * (mat + j);
			}
			delete[] mat;
			delete[] r;
			delete[] c;
			std::cout << "Dimension of matrices are not consistant"
			return -1;
		}
	}

	double* result = new double[r[0] * c[num - 1]];
	matrix_multiply(mat, result, r, c, num);

	std::cout << "Result = " << std::endl;
	displaymat(result, r[0], c[num - 1]);
	for (int i = 0; i < num; i++)
	{
		delete[] * (mat + i);
	}
	delete[] mat;
	delete[] result;
	delete[] r;
	delete[] c;
	
	return 0; 
}
