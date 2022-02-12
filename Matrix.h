#ifndef _MATRIX_H_
#define _MATRIX_H_

#pragma warning(disable : 4244)

// if you don't need matrix output to the console
// delete library iostream and method show, or comment it
#include <iostream>
#include <math.h>



template <typename T>
class Matrix
{
public:
	Matrix();
	Matrix(const size_t rows, const size_t columns);
	Matrix(const Matrix& other);
	Matrix(Matrix&& other);
	~Matrix();



	Matrix<T> operator=(const Matrix<T>& other);
	Matrix<T> operator=(	  Matrix<T>&& other);

	Matrix<T> operator+(const Matrix<T>& other);

	Matrix<T> operator*(const Matrix<T>& other);
	Matrix<T> operator*(const int value);
	Matrix<T> operator/(const int value);


	// returns transposed matrix
	Matrix<T> transpose();

	// returns adjoint matrix
	Matrix<T> adjoint();

	// returns inverse matrix
	Matrix<T> inverse();


	T		get_element(const int m, const int n) { return ptr[m][n]; };
	void	set_element(const int m, const int n, const T value) { ptr[m][n] = value; };

	size_t	get_nRows() { return rows_; };
	size_t	get_nColumns() { return columns_; };


	// outputs the matrix to the console
	void	show();
	void	clear();
	void	fill_rand_values();

	bool	is_square() { return rows_ == columns_; };
	bool	is_symetric();

	// clears old values and creates a matrix of new dimension
	void	recreate(const size_t rows, const size_t columns);


	// finds the determinant through decomposition on the first line 
	// and returns its value or returns 0 if matrix is not symmetric
	T		determinant();

protected:
	size_t	rows_;
	size_t	columns_;
	T**		ptr;

	T		determinant_;
	bool	det_is_found_;

	// returns minor for row and column element
	T		minor(const size_t row, const size_t column);
};



template<typename T>
inline Matrix<T>::Matrix()
	:
	rows_			(0),
	columns_		(0),
	ptr				(nullptr),
	determinant_	(0),
	det_is_found_	(false) {}

template<typename T>
inline Matrix<T>::Matrix(const size_t rows, const size_t columns)
	:
	rows_			(rows),
	columns_		(columns),
	determinant_	(0),
	det_is_found_	(false)
{
	ptr = new T * [rows_];
	for (size_t i = 0; i < rows_; ++i)
	{
		ptr[i] = new T[columns_];

		for (size_t j = 0; j < columns_; ++j)
		{
			ptr[i][j] = 0;
		}
	}
}

template<typename T>
inline Matrix<T>::Matrix(const Matrix& other)
	:
	rows_			(other.rows_),
	columns_		(other.columns_),
	determinant_	(other.determinant_),
	det_is_found_	(other.det_is_found_)
{
	ptr = new T * [rows_];
	for (size_t i = 0; i < rows_; ++i)
	{
		ptr[i] = new T[columns_];

		for (size_t j = 0; j < columns_; ++j)
		{
			ptr[i][j] = other.ptr[i][j];
		}
	}
}

template<typename T>
inline Matrix<T>::Matrix(Matrix&& other)
	:
	rows_			(other.rows_),
	columns_		(other.columns_),
	determinant_	(other.determinant_),
	det_is_found_	(other.det_is_found_)
{
	ptr			=	other.ptr;
	other.ptr	=	nullptr;
}

template<typename T>
inline Matrix<T>::~Matrix()
{
	clear();
}



template<typename T>
inline Matrix<T> Matrix<T>::operator=(const Matrix<T>& other)
{
	clear();

	rows_			=	other.rows_;
	columns_		=	other.columns_;
	determinant_	=	other.determinant_;
	det_is_found_	=	other.det_is_found_;

	ptr = new T * [rows_];
	for (size_t i = 0; i < rows_; ++i)
	{
		ptr[i] = new T[columns_];

		for (size_t j = 0; j < columns_; ++j)
		{
			ptr[i][j] = other.ptr[i][j];
		}
	}

	return *this;
}

template<typename T>
inline Matrix<T> Matrix<T>::operator=(		Matrix<T>&& other)
{
	clear();

	rows_			=	other.rows_;
	columns_		=	other.columns_;
	determinant_	=	other.determinant_;
	det_is_found_	=	other.det_is_found_;

	ptr				=	other.ptr;
	other.ptr		=	nullptr;

	return *this;
}

template<typename T>
inline Matrix<T> Matrix<T>::operator+(const Matrix<T>& other)
{
	if (rows_ != other.rows_ || columns_ != other.columns_)
	{
		return *this;
	}
	else
	{
		Matrix<T> C(rows_, columns_);

		for (size_t i = 0; i < rows_; ++i)
		{
			for (size_t j = 0; j < columns_; ++j)
			{
				C.ptr[i][j] = ptr[i][j] + other.ptr[i][j];
			}
		}

		return C;
	}

}

template<typename T>
inline Matrix<T> Matrix<T>::operator*(const Matrix<T>& other)
{
	if (columns_ != other.rows_)
	{
		return *this;
	}
	else
	{
		Matrix<T> C(rows_, other.columns_);

		for (size_t i = 0; i < rows_; ++i)
		{
			for (size_t k = 0; k < C.columns_; ++k)
			{
				for (size_t j = 0; j < columns_; ++j)
				{
					C.ptr[i][k] += ptr[i][j] * other.ptr[j][k];
				}
			}
		}

		return C;
	}
}

template<typename T>
inline Matrix<T> Matrix<T>::operator*(const int value)
{
	Matrix<T> C(rows_, columns_);

	for (size_t i = 0; i < rows_; ++i)
	{
		for (size_t j = 0; j < columns_; ++j)
		{
			C.ptr[i][j] = ptr[i][j] * value;
		}
	}

	return C;
}

template<typename T>
inline Matrix<T> Matrix<T>::operator/(const int value)
{
	Matrix<T> C(rows_, columns_);

	for (size_t i = 0; i < rows_; ++i)
	{
		for (size_t j = 0; j < columns_; ++j)
		{
			C.ptr[i][j] = ptr[i][j] / value;
		}
	}

	return C;
}


template<typename T>
inline Matrix<T> Matrix<T>::transpose()
{
	Matrix<T> C(columns_, rows_);

	for (size_t i = 0; i < rows_; ++i)
	{
		for (size_t j = 0; j < columns_; ++j)
		{
			C.ptr[j][i] = ptr[i][j];
		}
	}

	return C;
}

template<typename T>
inline Matrix<T> Matrix<T>::adjoint()
{
	if (determinant() != 0)
	{
		Matrix<T> adj(rows_, columns_); // its square matrix (rows == columns)

		for (size_t i = 0; i < rows_; ++i)
		{
			for (size_t j = 0; j < columns_; ++j)
			{
				adj.ptr[i][j] = pow((-1), i + j) * minor(i, j);
			}
		}

		return adj;
	}
	else
	{
		return *this;
	}
}

template<typename T>
inline Matrix<T> Matrix<T>::inverse()
{
	Matrix<T> inv(adjoint());

	inv = inv.transpose() / determinant();

	return inv;
}



template<typename T>
inline void Matrix<T>::show()
{
	for (size_t i = 0; i < rows_; ++i)
	{
		for (size_t j = 0; j < columns_; ++j)
		{
			std::cout << ptr[i][j] << "\t";
		}

		std::cout << "\n";
	}
}

template<typename T>
inline void Matrix<T>::clear()
{
	if (ptr != nullptr)
	{
		for (size_t i = 0; i < rows_; ++i)
		{
			delete[] ptr[i];
		}

		delete[] ptr;
		ptr = nullptr;
	}

	rows_ = columns_ = 0;
}

template<typename T>
inline void Matrix<T>::fill_rand_values()
{
	for (size_t i = 0; i < rows_; ++i)
	{
		for (size_t j = 0; j < columns_; ++j)
		{
			ptr[i][j] = rand() % 100;
		}
	}
}

template<typename T>
inline bool Matrix<T>::is_symetric()
{
	if (is_square())
	{
		for (size_t i = 0; i < rows_; ++i)
		{
			for (size_t j = 0; j < columns_; ++j)
			{
				if (ptr[i][j] != ptr[j][i])
				{
					return false;
				}
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}

template<typename T>
inline void Matrix<T>::recreate(const size_t rows, const size_t columns)
{
	clear();

	rows_		=	rows;
	columns_	=	columns;

	ptr = new T * [rows_];
	for (size_t i = 0; i < rows_; ++i)
	{
		ptr[i] = new T[columns_];

		for (size_t j = 0; j < columns_; ++j)
		{
			ptr[i][j] = 0;
		}
	}
}

template<typename T>
inline T Matrix<T>::determinant()
{
	if (det_is_found_)
	{
		return determinant_;
	}

	if (is_square())
	{
		// if the size is 1*1, the determinant is equal to the only member of the matrix
		if (rows_ == 1)
		{
			determinant_ = ptr[0][0];
			det_is_found_ = true;
			return determinant_;
		}
		// if the size of the matrix is 2*2, we can immediately find the determinant
		if (rows_ == 2)
		{
			determinant_ = (ptr[0][0] * ptr[1][1]) - (ptr[0][1] * ptr[1][0]);
			det_is_found_ = true;
			return determinant_;
		}

		// otherwise, we are looking for a determinant through decomposition by the first line
		for (size_t j = 0; j < columns_; ++j)
		{
			determinant_ += ptr[0][j] * pow((-1), j) * minor(0, j);
		}

		det_is_found_ = true;
		return determinant_;
	}
	else
	{
		return 0;
	}
}



template<typename T>
inline T Matrix<T>::minor(const size_t row, const size_t column)
{
	Matrix<T> minor_matrix(rows_ - 1, columns_ - 1);

	for (size_t i = 0, m = 0; i < rows_; ++i)
	{
		if (i == row)
		{
			++m;
			continue;
		}

		for (size_t j = 0, n = 0; j < columns_; ++j)
		{
			if (j == column)
			{
				++n;
				continue;
			}

			minor_matrix.set_element(i - m, j - n, ptr[i][j]);
		}
	}

	return minor_matrix.determinant();
}

#endif