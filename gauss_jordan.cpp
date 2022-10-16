#include <iostream>
#include <iomanip>

typedef unsigned int ui;
using namespace std;

#ifndef _USENRERRORCLASS_
#define throw(message) \
{printf("ERROR: %s\n     in file %s at line %d\n", message,__FILE__,__LINE__); throw(1);}
#else
struct NRerror {
	char *message;
	char *file;
	int line;
	NRerror(char *m, char *f, int l) : message(m), file(f), line(l) {}
};
#define throw(message) throw(NRerror(message,__FILE__,__LINE__));
void NRcatch(NRerror err) {
	printf("ERROR: %s\n     in file %s at line %d\n",
		err.message, err.file, err.line);
	exit(1);
}
#endif

void simplify(long* numerator, long* denominator, long divisor)
{
	if (divisor <= *numerator)
	{
		if (!(*numerator % divisor) && !(*denominator % divisor))
		{
			*numerator = *numerator / divisor;
			*denominator = *denominator / divisor;
			simplify(numerator, denominator, divisor);
		}
		else
		{
			simplify(numerator, denominator, divisor + 1);
		}
	}
}

class fraction
{
private:
	long numerator, denominator;
public:
	fraction(long numr = 0, long denomr = 1);
	friend fraction absolute(fraction& frac);
	fraction operator+(fraction& frac);
	fraction operator-(fraction& frac);
	fraction operator*(fraction& frac);
	fraction operator/(fraction& frac);
	friend bool operator>=(fraction frac1, fraction& farc2);
	friend bool operator==(fraction& frac1, fraction& frac2);
	long numerato_r();
	long denominato_r();
	friend std::ostream& operator<<(std::ostream& COUT, fraction& frac);
	~fraction();
};

fraction::fraction(long numr, long denomr) : numerator(numr), denominator(denomr) {}

fraction fraction::operator+(fraction& frac)
{
	fraction sum;
	sum.numerator = denominator * frac.numerator + numerator * frac.denominator;
	sum.denominator = frac.denominator * denominator;
	simplify(&sum.numerator, &sum.denominator, 2);
	if (sum.numerator < 0 && sum.denominator < 0)
	{
		sum.numerator = (-1)*sum.numerator;
		sum.denominator = (-1)*sum.denominator;
	}
	return sum;
}

fraction fraction::operator-(fraction& frac)
{
	fraction difference;
	difference.numerator = numerator * frac.denominator - denominator * frac.numerator;
	difference.denominator = frac.denominator * denominator;
	if (difference.numerator < 0 && difference.denominator < 0)
	{
		difference.numerator = (-1)*difference.numerator;
		difference.denominator = (-1)*difference.denominator;
	}
	simplify(&difference.numerator, &difference.denominator, 2);
	return difference;
}

fraction fraction::operator*(fraction& frac)
{
	fraction multiply;
	multiply.numerator = frac.numerator * numerator;
	multiply.denominator = frac.denominator * denominator;
	simplify(&multiply.numerator, &multiply.denominator, 2);
	if (multiply.numerator < 0 && multiply.denominator < 0)
	{
		multiply.numerator = (-1)*multiply.numerator;
		multiply.denominator = (-1)*multiply.denominator;
	}
	return multiply;
}

fraction fraction::operator/(fraction& frac)
{
	fraction division;
	division.numerator = frac.denominator * numerator;
	division.denominator = frac.numerator * denominator;
	simplify(&division.numerator, &division.denominator, 2);
	if (division.numerator < 0 && division.denominator < 0)
	{
		division.numerator = (-1)*division.numerator;
		division.denominator = (-1)*division.denominator;
	}
	return division;
}

long fraction::numerato_r()
{
	return numerator;
}

long fraction::denominato_r()
{
	return denominator;
}

bool operator>=(fraction frac1, fraction& frac2)
{
	bool compare;
	compare = frac1.numerator*frac2.denominator - frac1.denominator*frac2.numerator >= 0 ? true : false; 
	return compare;
}

bool operator==(fraction& frac1, fraction& frac2)
{
	bool compare;
	compare = (frac1.numerator * frac2.denominator == frac1.denominator * frac2.numerator) ? true : false;
	return compare;
}

fraction absolute(fraction& frac)
{
	fraction Abs; 
	Abs.numerator = frac.numerator < 0 ? (-1) * frac.numerator : frac.numerator;
	Abs.denominator = frac.denominator < 0 ? (-1) * frac.denominator : frac.denominator;
	return Abs;
}

std::ostream& operator<<(std::ostream& COUT, fraction& frac)
{
	COUT << frac.numerator << "/" << frac.denominator;
	return COUT;
}

fraction::~fraction() {}

inline void SWAP(fraction& a, fraction& b)
{
	fraction dum = a; a = b; b = dum;
}

class Matrix : public fraction
{
private:
	ui r;
	ui c;
	fraction** v;

public:
	Matrix();
	explicit Matrix(ui nr, ui mc);			// Zero-based array
	Matrix(ui nr, ui mc, const fraction& a);	//Initialize to constant
	Matrix(ui nr, ui mc, const fraction* a);	// Initialize to array
	Matrix(const Matrix& rhs);		// Copy constructor
	Matrix& operator=(const Matrix& rhs);	//assignment
	inline fraction* operator[](const ui i);	//subscripting: pouier to row i
	inline const fraction* operator[](const ui i) const;
	inline ui nrows() const;
	inline ui ncols() const;
	void resize(ui newr, ui newc); // resize (contents not preserved)
	void assign(ui newr, ui newc, const fraction& a); // resize and assign a constant value
	friend std::ostream& operator<<(std::ostream& o, fraction& mat);
	~Matrix();
};

Matrix::Matrix() : r(0), c(0), v(NULL) {}

Matrix::Matrix(ui nr, ui mc) : r(nr), c(mc), v(r > 0 ? new fraction * [nr] : NULL)
{
	ui i, nel = nr * mc;
	if (v) v[0] = nel > 0 ? new fraction[nel] : NULL;
	for (i = 1; i < nr; i++) v[i] = v[i - 1] + mc;
}

Matrix::Matrix(ui nr, ui mc, const fraction& a) : r(nr), c(mc), v(r > 0 ? new fraction * [nr] : NULL)
{
	ui i, j, nel = nr * mc;
	if (v) v[0] = nel > 0 ? new fraction[nel] : NULL;
	for (i = 1; i < nr; i++) v[i] = v[i - 1] + mc;
	for (i = 0; i < nr; i++) for (j = 0; j < mc; j++) v[i][j] = a;
}

Matrix::Matrix(ui nr, ui mc, const fraction* a) : r(nr), c(mc), v(r > 0 ? new fraction * [nr] : NULL)
{
	ui i, j, nel = nr * mc;
	if (v) v[0] = nel > 0 ? new fraction[nel] : NULL;
	for (i = 1; i < nr; i++) v[i] = v[i - 1] + mc;
	for (i = 0; i < nr; i++) for (j = 0; j < mc; j++) v[i][j] = *a++;
}

Matrix::Matrix(const Matrix& rhs) : r(rhs.r), c(rhs.c), v(r > 0 ? new fraction * [r] : NULL)
{
	ui i, j, nel = r * c;
	if (v) v[0] = nel > 0 ? new fraction[nel] : NULL;
	for (i = 1; i < r; i++) v[i] = v[i - 1] + c;
	for (i = 0; i < r; i++) for (j = 0; j < c; j++) v[i][j] = rhs[i][j];
}

Matrix& Matrix::operator=(const Matrix& rhs)
{
	if (this != &rhs)
	{
		ui i, j, nel;
		if (r != rhs.r || c != rhs.c)
		{
			if (v != NULL)
			{
				delete[](v[0]);
				delete[](v);
			}
			r = rhs.r;
			c = rhs.c;
			v = r > 0 ? new fraction * [r] : NULL;
			nel = r * c;
			if (v) v[0] = nel > 0 ? new fraction[nel] : NULL;
			for (i = 1; i < r; i++) v[i] = v[i - 1] + c;
		}
		for (i = 0; i < r; i++) for (j = 0; j < c; j++) v[i][j] = rhs[i][j];
	}
	return *this;
}

inline fraction* Matrix::operator[](const ui i)	//subscripting: pouier to row i
{
	return v[i];
}

inline const fraction* Matrix::operator[](const ui i) const
{
	return v[i];
}

inline ui Matrix::nrows() const
{
	return r;
}

inline ui Matrix::ncols() const
{
	return c;
}

void Matrix::resize(ui newr, ui newc)
{
	ui i, nel;
	if (newr != r || newc != c)
	{
		if (v != NULL)
		{
			delete[](v[0]);
			delete[](v);
		}
		r = newr;
		c = newc;
		v = r > 0 ? new fraction * [r] : NULL;
		nel = r * c;
		if (v) v[0] = nel > 0 ? new fraction[nel] : NULL;
		for (i = 1; i < r; i++) v[i] = v[i - 1] + c;
	}
}

void Matrix::assign(ui newr, ui newc, const fraction& a)
{
	ui i, j, nel;
	if (newr != r || newc != c)
	{
		if (v != NULL)
		{
			delete[](v[0]);
			delete[](v);
		}
		r = newr;
		c = newc;
		v = r > 0 ? new fraction * [r] : NULL;
		nel = r * c;
		if (v) v[0] = nel > 0 ? new fraction[nel] : NULL;
		for (i = 1; i < r; i++) v[i] = v[i - 1] + c;
	}
	for (i = 0; i < r; i++) for (j = 0; j < c; j++) v[i][j] = a;
}

Matrix::~Matrix()
{
	if (v != NULL)
	{
		delete[](v[0]);
		delete[](v);
	}
}

std::ostream& operator<<(std::ostream& o, Matrix& mat)
{
	for (ui i = 0; i < mat.nrows(); i++)
	{
		for (ui j = 0; j < mat.ncols(); j++)
		{
			o << std::right << std::setw(20) << mat[i][j];
		}
		o << std::endl;
	}

	return o;
}

void gaussj(Matrix& a, Matrix& b)
{
	int i, icol = 0, irow, j, k, l, ll, n = a.nrows(), m = b.ncols();
	fraction big, dum, pivinv, test, one(1,1), zero(0,1), save(0,1);

	int* indxc = new int[n];
	int* indxr = new int[n];
	int* ipiv = new int[n];

	for (ui j = 0; j < n; j++)
	{
		ipiv[j] = 0;
		indxc[j] = 0;
		indxr[j] = 0;
	}
	for (i = 0; i < n; i++) 
	{
		big = zero;
		for (j = 0; j < n; j++)
		{
			if (ipiv[j] != 1)
			{
				for (k = 0; k < n; k++) 
				{
					if (ipiv[k] == 0) 
					{
						if (absolute(a[j][k]) >= big) 
						{
							big = absolute(a[j][k]);
							irow = j;
							icol = k;
						}
					}
				}
			}
		}
		++(ipiv[icol]);
		if (irow != icol) 
		{
			for (l = 0; l < n; l++)
			{
				SWAP(a[irow][l], a[icol][l]);
			}
			
			for (l = 0; l < m; l++)
			{
				SWAP(b[irow][l], b[icol][l]);
			}
		}

		indxr[i] = irow;
		indxc[i] = icol;

		if (a[icol][icol] == test) throw("gaussj: Singular Matrix");
		pivinv = one / a[icol][icol];
		a[icol][icol] = one;
		for (l = 0; l < n; l++)
		{
			a[icol][l] = a[icol][l] * pivinv;
		}
		for (l = 0; l < m; l++)
		{
			b[icol][l] = b[icol][l] * pivinv;
		}
		for (ll = 0; ll < n; ll++)
		{
			if (ll != icol) {
				dum = a[ll][icol];
				a[ll][icol] = zero;
				for (l = 0; l < n; l++) 
				{
					test = a[icol][l] * dum;
					save = a[ll][l] - test;
					a[ll][l] = save;
					test = zero;
				}
				for (l = 0; l < m; l++) 
				{
					test = b[icol][l] * dum;
					b[ll][l] = b[ll][l] - test;
					test = zero;
				}
			}
		}
	}

	for (l = n - 1; l >= 0; l--) 
	{
		if (indxr[l] != indxc[l])
		{
			for (k = 0; k < n; k++)
			{
				SWAP(a[k][indxr[l]], a[k][indxc[l]]);
			}
		}
	}

	delete[]indxr;
	delete[]indxc;
	delete[]ipiv;
}

int main()
{
    fraction* a = new fraction[16];
	fraction* b = new fraction[4];

	long num_a[] = {-2, 0, 3, 0, 0, 1, 0, -3, 2, 3, -2, 0, 0, 0, -3, 2};
	long denom_a[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

	long num_b[] = {13, 0, 0, -15};
	long denom_b[] = {1, 1, 1, 1};

	for (ui i = 0; i < 4; i++)
	{
		for (ui j = 0; j < 4; j++)
		{
			a[4 * i + j] = fraction(num_a[4*i+j], denom_a[4*i+j]);
		}
		b[i] = fraction(num_b[i], denom_b[i]);
	}
	

	Matrix mat1(4,4,a);
	Matrix mat2(4,1,b);

	gaussj(mat1, mat2);

	delete []a;
	delete []b;

	cout << mat1 << "\n" << mat2 << endl;

	return 0;
}
