#pragma once

#include <exception>
#include <iostream>
#include <algorithm>
#include <type_traits>
#include <vector>

template<typename T = int>
class Matrix 
{
public:
	using valueType = T**;
	using sizeType = size_t;

	Matrix() = default;

	Matrix(sizeType r, sizeType c)
	{
		row = r;
		col = c;
		data = newMatrix(row, col);

		for (sizeType i = 0; i < r; i++)
		{
			for (sizeType j = 0; j < c; j++)
			{
				data[i][j] = 0;
			}
		}
	}

	Matrix(const Matrix& other)
	{
		row = other.row;
		col = other.col;
		data = newMatrix(row, col);

		for (sizeType i = 0; i < row; i++)
		{
			std::copy(other.data[i], other.data[i] + col, data[i]);
		}
	}

	Matrix(Matrix&& other) noexcept
	{
		row = std::exchange(other.row, 0);
		col = std::exchange(other.col, 0);
		data = std::exchange(other.data, nullptr);
	}

	~Matrix()
	{
		for (sizeType i = 0; i < row; i++) {
			delete[] data[i];
		}
		delete[] data;
	}

	Matrix& operator=(Matrix copy) 
	{
		copy.swap(*this);
		return *this;
	}

	Matrix operator+(const Matrix& other) const
	{
		if (row != other.row || col != other.col) {
			throw std::invalid_argument("Different dimensions.");
		}

		Matrix result(row, col);
		for (sizeType i = 0; i < row; i++)
		{
			for (sizeType j = 0; j < col; j++)
			{
				result[i][j] = data[i][j] + other.data[i][j];
			}
		}

		return result;
	}

	Matrix operator-(const Matrix& other) const
	{
		if (row != other.row || col != other.col) {
			throw std::invalid_argument("Different dimensions.");
		}

		Matrix result(row, col);
		for (sizeType i = 0; i < row; i++)
		{
			for (sizeType j = 0; j < col; j++)
			{
				result[i][j] = data[i][j] - other.data[i][j];
			}
		}

		return result;
	}

	void swap(Matrix& other) noexcept 
	{
		using std::swap;
		swap(row, other.row);
		swap(col, other.col);
		swap(data, other.data);
	}

	friend void swap(Matrix& a, Matrix& b) noexcept
	{
		a.swap(b);
	}

	void Resize(sizeType newRow, sizeType newCol)
	{
		valueType newData = newMatrix(newRow, newCol);

		int minRow = std::min(row, newRow);
		int minCol = std::min(col, newCol);

		for (sizeType i = 0; i < minRow; i++)
		{
			for (sizeType j = 0; j < minCol; j++)
			{
				newData[i][j] = data[i][j];
			}
		}

		delete[] data;
		row = newRow;
		col = newCol;
		data = newData;
	}

	T* operator[](int r)
	{
		return data[r];
	}

	const T* operator[](int r) const 
	{
		return data[r];
	}

	template<typename Compare = std::less<T>>
	void sortRows(Compare comp)
	{
		for (sizeType i = 0; i < row; i++)
		{
			std::sort(data[i], data[i] + col, comp);
		}
	}

	template<typename Compare = std::less<T>>
	void sortColumns(Compare comp)
	{
		for (sizeType j = 0; j < col; j++)
		{
			T* column = new T[row];
			for (sizeType i = 0; i < row; i++)
			{
				column[i] = data[i][j];
			}

			std::sort(column, column + row, comp);

			for (sizeType i = 0; i < row; i++)
			{
				data[i][j] = column[i];
			}

			delete[] column;
		}
	}

	int getRow() const { return row; }
	int getColumn() const { return col; }

	friend std::ostream& operator<<(std::ostream& os, const Matrix& m)
	{
		char sep = '|';

		if constexpr (std::is_integral<T>::value && !std::is_same<T, char>::value) 
		{
			sep = ';';
		}

		for (sizeType i = 0; i < m.row; i++)
		{
			for (sizeType j = 0; j < m.col; j++)
			{
				os << m.data[i][j] << sep << "\t";
			}
			os << std::endl;
		}

		return os;
	}

	friend std::istream& operator>>(std::istream& is, Matrix& m)
	{
		for (sizeType i = 0; i < m.row; i++)
		{
			for (sizeType j = 0; j < m.col; j++)
			{
				is >> m.data[i][j];
			}
		}

		return is;
	}

	sizeType calculateRank() const 
	{
		sizeType rank = 0;
		Matrix<T> copy(*this);

		for (sizeType i = 0; i < col; i++)
		{
			sizeType pivot_row = rank;
			while (pivot_row < row && copy.data[pivot_row][i] == 0) 
			{
				pivot_row++;
			}

			if (pivot_row == row) 
			{
				continue;
			}

			if (pivot_row != rank) 
			{
				std::swap(copy.data[pivot_row], copy.data[rank]);
			}

			for (sizeType j = rank + 1; j < row; j++)
			{
				T factor = copy.data[j][i] / copy.data[rank][i];

				for (sizeType k = i; k < col; k++)
				{
					copy.data[j][k] -= factor * copy.data[rank][k];
				}
			}

			rank++;
		}

		return rank;
	}

	// I know it's a quite strange way to check
	//  if matrix is const but it's the only way that works
	// (tried if_enable and is_const in previous method)
	sizeType calculateRank()
	{
		throw std::runtime_error("Matrix is not const");
		return 0;
	}

private:
	sizeType row = 0;
	sizeType col = 0;
	valueType data = nullptr;

	valueType newMatrix(sizeType r, sizeType c) const
	{
		auto m = new T*[r];

		for (sizeType i = 0; i < r; i++)
		{
			m[i] = new T[c];
		}

		return m;
	}
};

template<>
class Matrix<std::string>
{
public: 
	using ValueType = std::string;
	using PointerType = ValueType*;
	using MatrixType = ValueType**;
	using sizeType = size_t;

	Matrix() = default;

	Matrix(sizeType r, sizeType c)
	{
		row = r;
		col = c;
		data = newMatrix(row, col);

		for (sizeType i = 0; i < r; i++)
		{
			for (sizeType j = 0; j < c; j++)
			{
				data[i][j] = "0";
			}
		}
	}

	Matrix(const Matrix & other)
	{
		row = other.row;
		col = other.col;
		data = newMatrix(row, col);

		for (sizeType i = 0; i < row; i++)
		{
			std::copy(other.data[i], other.data[i] + col, data[i]);
		}
	}

	Matrix(Matrix && other) noexcept
	{
		row = std::exchange(other.row, 0);
		col = std::exchange(other.col, 0);
		data = std::exchange(other.data, nullptr);
	}

	~Matrix()
	{
		for (sizeType i = 0; i < row; i++) {
			delete[] data[i];
		}
		delete[] data;
	}

	Matrix<ValueType>& operator=(Matrix copy)
	{
		copy.swap(*this);
		return *this;
	}

	Matrix<ValueType> operator+(const Matrix & other) const
	{
		if (row != other.row || col != other.col) {
			throw std::invalid_argument("Different dimensions.");
		}

		Matrix result(row, col);
		for (sizeType i = 0; i < row; i++) 
		{
			for (sizeType j = 0; j < col; j++) 
			{
				result[i][j] = data[i][j] + other.data[i][j];
			}
		}

		return result;
	}

	void swap(Matrix< ValueType>& other) noexcept
	{
		using std::swap;
		swap(row, other.row);
		swap(col, other.col);
		swap(data, other.data);
	}

	friend void swap(Matrix<ValueType>& a, Matrix<ValueType>& b) noexcept
	{
		a.swap(b);
	}

	void Resize(sizeType newRow, sizeType newCol)
	{
		MatrixType newData = newMatrix(newRow, newCol);

		sizeType minRow = std::min(row, newRow);
		sizeType minCol = std::min(col, newCol);

		for (sizeType i = 0; i < minRow; i++)
		{
			for (sizeType j = 0; j < minCol; j++)
			{
				newData[i][j] = data[i][j];
			}
		}

		delete[] data;
		row = newRow;
		col = newCol;
		data = newData;
	}

	PointerType operator[](int r)
	{
		return data[r];
	}

	const PointerType operator[](int r) const
	{
		return data[r];
	}

	template<typename Compare = std::less<>>
	void sortRows(Compare comp)
	{
		for (sizeType i = 0; i < row; i++)
		{
			std::sort(data[i], data[i] + col, comp);
		}
	}

	template<typename Compare = std::less<>>
	void sortColumns(Compare comp)
	{
		for (sizeType j = 0; j < col; j++)
		{
			PointerType column = new ValueType[row];
			for (sizeType i = 0; i < row; i++)
			{
				column[i] = data[i][j];
			}

			std::sort(column, column + row, comp);

			for (sizeType i = 0; i < row; i++)
			{
				data[i][j] = column[i];
			}

			delete[] column;
		}
	}

	int getRow() const { return row; }
	int getColumn() const { return col; }

	friend std::ostream& operator<<(std::ostream & os, const Matrix<ValueType>& m)
	{
		for (sizeType i = 0; i < m.row; i++)
		{
			for (sizeType j = 0; j < m.col; j++)
			{
				os << m.data[i][j] << " | " << "\t";
			}
			os << std::endl;
		}

		return os;
	}

	friend std::istream& operator>>(std::istream & is, Matrix<ValueType>& m)
	{
		for (sizeType i = 0; i < m.row; i++)
		{
			for (sizeType j = 0; j < m.col; j++)
			{
				is >> m.data[i][j];
			}
		}

		return is;
	}

private:
	sizeType row = 0;
	sizeType col = 0;
	MatrixType data = nullptr;

	MatrixType newMatrix(sizeType r, sizeType c) const
	{
		auto m = new PointerType[r];

		for (sizeType i = 0; i < r; i++)
		{
			m[i] = new ValueType[c];
		}

		return m;
	}
};

using MatrixI = Matrix<int>;
using MatrixF = Matrix<float>;
using MatrixD = Matrix<double>;
using MatrixS = Matrix<std::string>;
using MatrixC = Matrix<char>;