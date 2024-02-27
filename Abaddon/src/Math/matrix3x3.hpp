#pragma once
#include <array>
#include <math.h>
#include "vector3.hpp"

#define degreesToRadiansMultiplier 0.01745329251f

namespace math
{
	template <class T>
	class matrix3x3
	{
	public:
		matrix3x3<T>();
		matrix3x3<T>(const matrix3x3<T>& aMatrix);

		void Transpose();
		void RotateAroundX(T aAngleInDegrees);
		void RotateAroundY(T aAngleInDegrees);

		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;
		void operator=(const matrix3x3<T>& aMatrix);

	private:
		std::array<std::array<T, 3>, 3> myMatrix3x3;
	};

#pragma region Constructors
	template <class T>
	matrix3x3<T>::matrix3x3()
	{
		myMatrix3x3 = {};

		for (int row = 0; row < 3; row++)
		{
			for (int column = 0; column < 3; column++)
			{
				if (row == column)
				{
					myMatrix3x3[row][column] = 1;
				}
			}
		}
	}

	template <class T>
	matrix3x3<T>::matrix3x3(const matrix3x3<T>& aMatrix)
	{
		myMatrix3x3 = aMatrix.myMatrix3x3;
	}
#pragma endregion

#pragma region Operators
	template <class T>
	T& matrix3x3<T>::operator()(const int aRow, const int aColumn)
	{
		return myMatrix3x3[aRow][aColumn];
	}

	template <class T>
	const T& matrix3x3<T>::operator()(const int aRow, const int aColumn) const
	{
		return myMatrix3x3[aRow][aColumn];
	}

	template <class T>
	matrix3x3<T> operator+(const matrix3x3<T>& aMatrix1, const matrix3x3<T>& aMatrix2)
	{
		matrix3x3<T> matrix = {};

		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 3; c++)
			{
				matrix(r,c) = aMatrix1(r,c) + aMatrix2(r,c);
			}
		}

		return matrix;
	}

	template <class T>
	matrix3x3<T> operator-(const matrix3x3<T>& aMatrix1, const matrix3x3<T>& aMatrix2)
	{
		matrix3x3<T> matrix = {};

		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 3; c++)
			{
				matrix(r,c) = aMatrix1(r,c) - aMatrix2(r,c);
			}
		}

		return matrix;
	}

	template <class T>
	matrix3x3<T> operator*(const matrix3x3<T>& aMatrix1, const matrix3x3<T>& aMatrix2)
	{
		matrix3x3<T> matrix = {};

		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 3; c++)
			{
				matrix(r, c) = aMatrix1(0+r,0) * aMatrix2(0,0+c) + aMatrix1(0+r,1) * aMatrix2(1,0+c) + aMatrix1(0+r,2) * aMatrix2(2,0+c);
			}
		}

		return matrix;
	}

	template <class T>
	vector3<T> operator*(const vector3<T>& aVector, const matrix3x3<T>& aMatrix)
	{
		vector3<T> vector = {};

		vector.x = aMatrix(0, 0) * aVector.x + aMatrix(0, 1) * aVector.y + aMatrix(0, 2) * aVector.z;
		vector.y = aMatrix(1, 0) * aVector.x + aMatrix(1, 1) * aVector.y + aMatrix(1, 2) * aVector.z;
		vector.z = aMatrix(2, 0) * aVector.x + aMatrix(2, 1) * aVector.y + aMatrix(2, 2) * aVector.z;

		return vector;
	}

	template <class T>
	void operator+=(matrix3x3<T>& aMatrix1, const matrix3x3<T>& aMatrix2)
	{
		aMatrix1 = aMatrix1 + aMatrix2;
	}

	template <class T>
	void operator-=(matrix3x3<T>& aMatrix1, const matrix3x3<T>& aMatrix2)
	{
		aMatrix1 = aMatrix1 - aMatrix2;
	}

	template <class T>
	void operator*=(matrix3x3<T>& aMatrix1, const matrix3x3<T>& aMatrix2)
	{
		aMatrix1 = aMatrix1 * aMatrix2;
	}

	template <class T>
	void matrix3x3<T>::operator=(const matrix3x3<T>& aMatrix)
	{
		myMatrix3x3 = aMatrix.myMatrix3x3;
	}

	template <class T>
	bool operator==(const matrix3x3<T>& aMatrix1, const matrix3x3<T>& aMatrix2)
	{
		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 3; c++)
			{
				if (aMatrix1(r,c) != aMatrix2(r,c))
					return false;
			}
		}

		return true;
	}

	template <class T>
	bool operator!=(const matrix3x3<T>& aMatrix1, const matrix3x3<T>& aMatrix2)
	{
		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 3; c++)
			{
				if (aMatrix1(r,c) != aMatrix2(r,c))
					return true;
			}
		}

		return false;
	}
#pragma endregion

#pragma region Methods
	template <class T>
	void matrix3x3<T>::Transpose()
	{
		matrix3x3<T> tempMatrix;
		tempMatrix.myMatrix3x3 = myMatrix3x3;

		myMatrix3x3[0][1] = tempMatrix(1,0);
		myMatrix3x3[0][2] = tempMatrix(2,0);
		myMatrix3x3[1][2] = tempMatrix(2,1);

		myMatrix3x3[1][0] = tempMatrix(0, 1);
		myMatrix3x3[2][0] = tempMatrix(0, 2);
		myMatrix3x3[2][1] = tempMatrix(1, 2);
	}

	template <class T>
	void matrix3x3<T>::RotateAroundX(T aAngleInDegrees)
	{
		T angle = aAngleInDegrees * (T)degreesToRadiansMultiplier;

		matrix3x3<T> rotationMatrix;
		rotationMatrix(0,0) = std::cos(angle);
		rotationMatrix(0,1) = -std::sin(angle);
		rotationMatrix(1,0) = std::sin(angle);
		rotationMatrix(1,1) = std::cos(angle);

		*this = *this * rotationMatrix;
	}

	template <class T>
	void matrix3x3<T>::RotateAroundY(T aAngleInDegrees)
	{
		T angle = aAngleInDegrees * (T)degreesToRadiansMultiplier;

		matrix3x3<T> rotationMatrix;
		rotationMatrix(0, 0) = std::cos(angle);
		rotationMatrix(0, 1) = -std::sin(angle);
		rotationMatrix(1, 0) = std::sin(angle);
		rotationMatrix(1, 1) = std::cos(angle);

		*this = *this * rotationMatrix;
	}
#pragma endregion
}