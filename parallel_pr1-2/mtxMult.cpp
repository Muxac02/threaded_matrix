#include "mtxMult.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

std::mutex matr_latch;

using namespace std::chrono_literals;

void matrixMultiply(int** a, int** b, int n, int** res)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int* column = getColumn(b, n, j);
			res[i][j] = matrixMultRowOnColumn(a[i], column, n);
			delete column;
		}
	}
	outMatr(res,n);
}

int* getColumn(int** a, int n, int j)
{
	int* res = new int[n];
	for (int i=0;i<n;i++)
	{
		res[i] = a[i][j];
	}
	return res;
}

int matrixMultRowOnColumn(int* a, int* b, int n)
{
	int res = 0;
	for (int i = 0; i < n; i++)
	{
		res += a[i] * b[i];
	}
	return res;
}

void outMatr(int** a, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			std::cout << a[i][j]<<" ";
		}
		std::cout << std::endl;
	}
}

Matrix::Matrix(bool fillWithRandomElements)
{
	std::cout << "¬ведите размерность матрицы (строки, столбцы)";
	std::cin >> this->rows>>this->columns;
	this->els = new int* [this->rows];
	for (int i = 0; i < this->rows; i++)
	{
		this->els[i] = new int[this->columns];
	}
	if (!fillWithRandomElements)
	{
		for (int i = 0; i < this->rows; i++)
		{
			for (int j = 0; j < this->columns; j++)
			{
				std::cout << "¬ведите значение элемента M[" << i << "][" << j << "] = ";
				std::cin >> this->els[i][j];
			}
		}
	}
	else
	{
		this->fillMatrixWithRandomElemets();
	}
}

Matrix::Matrix(int r, int c, bool fillWithRandomElements)
{
	this->rows = r;
	this->columns = c;
	this->els = new int* [this->rows];
	for (int i = 0; i < this->rows; i++)
	{
		this->els[i] = new int[this->columns];
	}
	if (!fillWithRandomElements)
	{
		for (int i = 0; i < this->rows; i++)
		{
			for (int j = 0; j < this->columns; j++)
			{
				std::cout << "¬ведите значение элемента M[" << i << "][" << j << "] = ";
				std::cin >> this->els[i][j];
			}
		}
	}
	else
	{
		this->fillMatrixWithRandomElemets();
	}
}

Matrix::Matrix(int** els, int r, int c, bool fillWithRandomElements)
{
	this->rows = r;
	this->columns = c;
	this->els = new int* [this->rows];
	for (int i = 0; i < this->rows; i++)
	{
		this->els[i] = new int[this->columns];
	}
	if (!fillWithRandomElements)
	{
		for (int i = 0; i < this->rows; i++)
		{
			for (int j = 0; j < this->columns; j++)
			{
				this->els[i][j] = els[i][j];
			}
		}
	}
	else
	{
		this->fillMatrixWithRandomElemets();
	}
}

Matrix::Matrix(const Matrix& m)
{
	this->clearElsMem();
	this->rows = m.rows;
	this->columns = m.columns;
	this->els = new int* [this->rows];
	for (int i = 0; i < this->rows; i++)
	{
		this->els[i] = new int[this->columns];
	}
	for (int i = 0; i < this->rows; i++)
	{
		for (int j = 0; j < this->columns; j++)
		{
			this->els[i][j] = m.els[i][j];
		}
	}
}

Matrix::~Matrix()
{
	for (int i = 0; i < this->rows; i++)
	{
		delete[]this->els[i];
	}
	//delete[]this->els;
}

void Matrix::fillMatrixWithRandomElemets()
{
	srand(time(0));
	for (int i = 0; i < this->rows; i++)
	{
		for (int j = 0; j < this->columns; j++)
		{
			this->els[i][j] = rand() % 1000;
		}
	}
}

void Matrix::clearElsMem()
{
	for (int i = 0; i < this->rows; i++)
	{
		delete[]this->els[i];
	}
	//delete[]this->els;
}

int* Matrix::getColumn(int j) const
{
	int* res = new int[this->rows];
	for (int i = 0; i < this->rows; i++)
	{
		res[i] = this->els[i][j];
	}
	return res;
}

int Matrix::matrixMultRowOnColumn(int* a, int* b)
{
	int res = 0;
	for (int i = 0; i < this->rows; i++)
	{
		res += a[i] * b[i];
	}
	return res;
}

void Matrix::outMatr()
{
	//matr_latch.lock();
	for (int i = 0; i < this->rows; i++)
	{
		for (int j = 0; j < this->columns; j++)
		{
			std::cout << this->els[i][j] << " ";
		}
		std::cout << std::endl;
	}
	//matr_latch.unlock();
}

void Matrix::outMatr(Matrix& m)
{
	for (int i = 0; i < m.rows; i++)
	{
		for (int j = 0; j < m.columns; j++)
		{
			std::cout << m.els[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void Matrix::staticOutMatr(Matrix& m)
{
	for (int i = 0; i < m.rows; i++)
	{
		for (int j = 0; j < m.columns; j++)
		{
			std::cout << m.els[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

Matrix Matrix::operator+(const Matrix& m)
{
	std::cout << "Sum start on thread id: " <<std::this_thread::get_id()<< std::endl;
	Matrix result(this->els, this->columns, m.rows);
	for (int i = 0; i < this->rows; i++)
	{
		for (int j = 0; j < this->columns; j++)
		{
			result.els[i][j] = this->els[i][j] + m.els[i][j];
			std::this_thread::sleep_for(50ms);
		}
	}
	std::cout << "Sum ended on thread id: " << std::this_thread::get_id() << std::endl;
	return result;
}

Matrix& Matrix::operator=(const Matrix& m)
{
	if (&m!=this)
	{
		this->clearElsMem();
		this->rows = m.rows;
		this->columns = m.columns;
		for (int i = 0; i < this->rows; i++)
		{
			this->els[i] = new int[this->columns];
		}
		for (int i = 0; i < this->rows; i++)
		{
			for (int j = 0; j < this->columns; j++)
			{
				this->els[i][j] = m.els[i][j];
			}
		}
	}
	return *this;
}

Matrix Matrix::operator-(const Matrix& m)
{
	std::cout << "Sub start on thread id: " << std::this_thread::get_id() << std::endl;
	Matrix result(this->els, this->columns, m.rows);
	for (int i = 0; i < this->rows; i++)
	{
		for (int j = 0; j < this->columns; j++)
		{
			result.els[i][j] = this->els[i][j] - m.els[i][j];
			std::this_thread::sleep_for(50ms);
		}
	}
	std::cout << "Sub ended on thread id: " << std::this_thread::get_id() << std::endl;
	return result;
}

Matrix Matrix::operator*(const Matrix& m)
{
	std::cout << "Mult start on thread id: " << std::this_thread::get_id() << std::endl;
	Matrix result(this->els, this->columns, m.rows);
	for (int i = 0; i < m.rows; i++)
	{
		for (int j = 0; j < this->columns; j++)
		{
			int* column = m.getColumn(j);
			result.els[i][j] = matrixMultRowOnColumn(this->els[i], column);
			delete column;
			std::this_thread::sleep_for(50ms);
		}
	}
	std::cout << "Mult ended on thread id: " << std::this_thread::get_id() << std::endl;
	return result;
}

void Matrix::matrMult(Matrix& m1, Matrix& m2, Matrix& result)
{
	if (result.columns!=m1.columns&&result.rows!=m2.rows) {
		for (int i = 0; i < result.rows; i++) {
			delete[] result.els[i];
		}
		delete[] result.els;
		result.columns = m1.columns;
		result.rows = m2.rows;
		result.els = new int* [result.rows];
		for (int i = 0; i < result.rows;i++) {
			result.els[i] = new int[result.columns];
		}
	}
	for (int i = 0; i < m2.rows; i++)
	{
		for (int j = 0; j < m1.columns; j++)
		{
			int* column = m2.getColumn(j);
			result.els[i][j] = matrixMultRowOnColumn(m1.els[i], column);
			delete column;
		}
	}
}