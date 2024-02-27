#pragma once
#include <array>
#include <math.h>
#include "vector4.hpp"

#define degreesToRadiansMultiplier 0.01745329251f

namespace math
{
	template <class T>
	class matrix4x4
	{
	public:
		matrix4x4<T>();
		matrix4x4<T>(const matrix4x4<T>& aMatrix);

		void Transpose();
		void RotateAroundX(T aAngleInDegrees);
		void RotateAroundY(T aAngleInDegrees);
		void RotateAroundZ(T aAngleInDegrees);
		void Inverse();

		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;
		void operator=(const matrix4x4<T>& aMatrix);

	private:
		std::array<std::array<T, 4>, 4> myMatrix4x4;
	};

#pragma region Constructors
	template <class T>
	matrix4x4<T>::matrix4x4()
	{
		myMatrix4x4 = {};

		for (int row = 0; row < 4; row++)
		{
			for (int column = 0; column < 4; column++)
			{
				if (row == column)
				{
					myMatrix4x4[row][column] = 1;
				}
			}
		}
	}

	template <class T>
	matrix4x4<T>::matrix4x4(const matrix4x4<T>& aMatrix)
	{
		myMatrix4x4 = aMatrix.myMatrix4x4;
	}
#pragma endregion

#pragma region Operators
	template <class T>
	T& matrix4x4<T>::operator()(const int aRow, const int aColumn)
	{
		return myMatrix4x4[aRow][aColumn];
	}

	template <class T>
	const T& matrix4x4<T>::operator()(const int aRow, const int aColumn) const
	{
		return myMatrix4x4[aRow][aColumn];
	}

	template <class T>
	matrix4x4<T> operator+(const matrix4x4<T>& aMatrix1, const matrix4x4<T>& aMatrix2)
	{
		matrix4x4<T> matrix = {};

		for (int r = 0; r < 4; r++)
		{
			for (int c = 0; c < 4; c++)
			{
				matrix(r, c) = aMatrix1(r, c) + aMatrix2(r, c);
			}
		}

		return matrix;
	}

	template <class T>
	matrix4x4<T> operator-(const matrix4x4<T>& aMatrix1, const matrix4x4<T>& aMatrix2)
	{
		matrix4x4<T> matrix = {};

		for (int r = 0; r < 4; r++)
		{
			for (int c = 0; c < 4; c++)
			{
				matrix(r, c) = aMatrix1(r, c) - aMatrix2(r, c);
			}
		}

		return matrix;
	}

	template <class T>
	matrix4x4<T> operator*(const matrix4x4<T>& aMatrix1, const matrix4x4<T>& aMatrix2)
	{
		matrix4x4<T> matrix = {};

		for (int r = 0; r < 4; r++)
		{
			for (int c = 0; c < 4; c++)
			{
				matrix(r, c) = aMatrix1(0 + r, 0) * aMatrix2(0, 0 + c) + aMatrix1(0 + r, 1) * aMatrix2(1, 0 + c) + aMatrix1(0 + r, 2) * aMatrix2(2, 0 + c) + aMatrix1(0 + r, 3) * aMatrix2(3, 0 + c);
			}
		}

		return matrix;
	}

	template <class T>
	vector4<T> operator*(const vector4<T>& aVector, const matrix4x4<T>& aMatrix)
	{
		vector4<T> vector = {};

		vector.x = aMatrix(0, 0) * aVector.x + aMatrix(0, 1) * aVector.y + aMatrix(0, 2) * aVector.z + aMatrix(0, 3) * aVector.w;
		vector.y = aMatrix(1, 0) * aVector.x + aMatrix(1, 1) * aVector.y + aMatrix(1, 2) * aVector.z + aMatrix(1, 3) * aVector.w;
		vector.z = aMatrix(2, 0) * aVector.x + aMatrix(2, 1) * aVector.y + aMatrix(2, 2) * aVector.z + aMatrix(2, 3) * aVector.w;
		vector.w = aMatrix(3, 0) * aVector.x + aMatrix(3, 1) * aVector.y + aMatrix(3, 2) * aVector.z + aMatrix(3, 3) * aVector.w;

		return vector;
	}

	template <class T>
	void operator+=(matrix4x4<T>& aMatrix1, const matrix4x4<T>& aMatrix2)
	{
		aMatrix1 = aMatrix1 + aMatrix2;
	}

	template <class T>
	void operator-=(matrix4x4<T>& aMatrix1, const matrix4x4<T>& aMatrix2)
	{
		aMatrix1 = aMatrix1 - aMatrix2;
	}

	template <class T>
	void operator*=(matrix4x4<T>& aMatrix1, const matrix4x4<T>& aMatrix2)
	{
		aMatrix1 = aMatrix1 * aMatrix2;
	}

	template <class T>
	void matrix4x4<T>::operator=(const matrix4x4<T>& aMatrix)
	{
		myMatrix4x4 = aMatrix.myMatrix4x4;
	}

	template <class T>
	bool operator==(const matrix4x4<T>& aMatrix1, const matrix4x4<T>& aMatrix2)
	{
		for (int r = 0; r < 4; r++)
		{
			for (int c = 0; c < 4; c++)
			{
				if (aMatrix1(r, c) != aMatrix2(r, c))
					return false;
			}
		}

		return true;
	}

	template <class T>
	bool operator!=(const matrix4x4<T>& aMatrix1, const matrix4x4<T>& aMatrix2)
	{
		for (int r = 0; r < 4; r++)
		{
			for (int c = 0; c < 4; c++)
			{
				if (aMatrix1(r, c) != aMatrix2(r, c))
					return true;
			}
		}

		return false;
	}
#pragma endregion

#pragma region Methods
	template <class T>
	void matrix4x4<T>::Transpose()
	{
		matrix4x4<T> tempMatrix;
		tempMatrix.myMatrix4x4 = myMatrix4x4;

		myMatrix4x4[0][1] = tempMatrix(1, 0);
		myMatrix4x4[0][2] = tempMatrix(2, 0);
		myMatrix4x4[0][3] = tempMatrix(3, 0);
		myMatrix4x4[1][2] = tempMatrix(2, 1);
		myMatrix4x4[1][3] = tempMatrix(3, 1);
		myMatrix4x4[2][3] = tempMatrix(3, 2);

		myMatrix4x4[1][0] = tempMatrix(0, 1);
		myMatrix4x4[2][0] = tempMatrix(0, 2);
		myMatrix4x4[3][0] = tempMatrix(0, 3);
		myMatrix4x4[2][1] = tempMatrix(1, 2);
		myMatrix4x4[3][1] = tempMatrix(1, 3);
		myMatrix4x4[3][2] = tempMatrix(2, 3);
	}

	template <class T>
	void matrix4x4<T>::RotateAroundX(T aAngleInDegrees)
	{
		T angle = aAngleInDegrees * (T)degreesToRadiansMultiplier;

		matrix4x4<T> rotationMatrix;
		rotationMatrix(1, 1) = std::cos(angle);
		rotationMatrix(1, 2) = -std::sin(angle);
		rotationMatrix(2, 1) = std::sin(angle);
		rotationMatrix(2, 2) = std::cos(angle);

		*this = *this * rotationMatrix;
	}

	template <class T>
	void matrix4x4<T>::RotateAroundY(T aAngleInDegrees)
	{
		T angle = aAngleInDegrees * (T)degreesToRadiansMultiplier;

		matrix4x4<T> rotationMatrix;
		rotationMatrix(0, 0) = std::cos(angle);
		rotationMatrix(0, 2) = std::sin(angle);
		rotationMatrix(2, 0) = -std::sin(angle);
		rotationMatrix(2, 2) = std::cos(angle);

		*this = *this * rotationMatrix;
	}

	template <class T>
	void matrix4x4<T>::RotateAroundZ(T aAngleInDegrees)
	{
		T angle = aAngleInDegrees * (T)degreesToRadiansMultiplier;

		matrix4x4<T> rotationMatrix;
		rotationMatrix(0, 0) = std::cos(angle);
		rotationMatrix(0, 1) = -std::sin(angle);
		rotationMatrix(1, 0) = std::sin(angle);
		rotationMatrix(1, 1) = std::cos(angle);

		*this = *this * rotationMatrix;
	}

	template <class T>
	void matrix4x4<T>::Inverse()
	{
		matrix4x4<T> tempRotationMatrix;

		for (short r = 0; r < 3; ++r)
		{
			for (short c = 0; c < 3; ++c)
			{
				tempRotationMatrix(r,c) = myMatrix4x4[c][r];
			}
		}

		matrix4x4<T> tempTranslationMatrix;
		for (short c = 0; c < 3; ++c)
		{
			tempTranslationMatrix(2,c) = myMatrix4x4[2][c] * -1;
		}

		*this = tempTranslationMatrix * tempRotationMatrix;
	}
#pragma endregion
}