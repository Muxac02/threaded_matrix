#pragma once
void matrixMultiply(int**a,int**b,int n, int**res);
int* getColumn(int** a, int n, int j);
int matrixMultRowOnColumn(int*a, int*b, int n);
void outMatr(int** a, int n);

class Matrix
{
	int** els;
	int columns;
	int rows;
public:
	Matrix(bool fillWithRandomElements = false);
	Matrix(int r, int c, bool fillWithRandomElements = false);
	Matrix(int** els, int r, int c, bool fillWithRandomElements = false);
	Matrix(const Matrix& m);
	~Matrix();
	void fillMatrixWithRandomElemets();
	void clearElsMem();
	int* getColumn(int j) const;
	int matrixMultRowOnColumn(int* a, int* b);
	void outMatr();
	void outMatr(Matrix& m);
	static void staticOutMatr(Matrix& m);
	Matrix operator+(const Matrix& m);
	Matrix& operator=(const Matrix& m);
	Matrix operator-(const Matrix& m);
	Matrix operator*(const Matrix& m);
	void matrMult(Matrix& m1, Matrix& m2, Matrix& result);
};