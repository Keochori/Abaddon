#pragma once
#include <cassert>
#include <initializer_list>

namespace math
{
	template<class T>
	class vector3
	{
	public:
		T x;
		T y;
		T z;

		vector3<T>();
		vector3<T>(T aX, T aY, T aZ);
		~vector3<T>() = default;

		T Length() const;
		T LengthSqr() const;
		T Dot(const vector3<T>& aOtherVector) const;
		vector3<T> Cross(const vector3<T>& aOtherVector) const;
		void Normalize();
		vector3<T> GetNormalized() const;

		vector3<T>& operator=(const vector3<T>& aOtherVector) = default;
	};

#pragma region Constructors
	template<class T>
	vector3<T>::vector3()
	{
		x = {};
		y = {};
		z = {};
	}

	template<class T>
	vector3<T>::vector3(T aX, T aY, T aZ)
	{
		x = aX;
		y = aY;
		z = aZ;
	}
#pragma endregion

#pragma region Operators
	template <class T>
	vector3<T> operator+(const vector3<T>& aVector1, const vector3<T>& aVector2)
	{
		return vector3<T>(aVector1.x + aVector2.x, aVector1.y + aVector2.y, aVector1.z + aVector2.z);
	}

	template<class T>
	vector3<T> operator-(const vector3<T>& aVector1, const vector3<T>& aVector2)
	{
		return vector3<T>(aVector1.x - aVector2.x, aVector1.y - aVector2.y, aVector1.z - aVector2.z);
	}

	template<class T>
	vector3<T> operator*(const vector3<T>& aVector, const T& aScalar)
	{
		return vector3<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar);
	}

	template<class T>
	vector3<T> operator/(const vector3<T>& aVector, const T& aScalar)
	{
		assert(aScalar > 0 && "Can't divide vector with 0.");
		T multiplicationValue = 1 / aScalar;
		return vector3<T>(aVector.x * multiplicationValue, aVector.y * multiplicationValue, aVector.z * multiplicationValue);
	}

	template<class T>
	void operator+=(vector3<T>& aVector1, const vector3<T>& aVector2)
	{
		aVector1.x = aVector1.x + aVector2.x;
		aVector1.y = aVector1.y + aVector2.y;
		aVector1.z = aVector1.z + aVector2.z;
	}

	template<class T>
	void operator-=(vector3<T>& aVector1, const vector3<T>& aVector2)
	{
		aVector1.x = aVector1.x - aVector2.x;
		aVector1.y = aVector1.y - aVector2.y;
		aVector1.z = aVector1.z - aVector2.z;
	}

	template<class T>
	void operator*=(vector3<T>& aVector, const T& aScalar)
	{
		aVector.x = aVector.x * aScalar;
		aVector.y = aVector.y * aScalar;
		aVector.z = aVector.z * aScalar;
	}

	template<class T>
	void operator/=(vector3<T>& aVector, const T& aScalar)
	{
		assert(aScalar > 0 && "Can't divide vector with 0.");
		T multiplicationValue = 1 / aScalar;
		aVector.x = aVector.x * multiplicationValue;
		aVector.y = aVector.y * multiplicationValue;
		aVector.z = aVector.z * multiplicationValue;
	}

	template<class T>
	bool operator==(const vector3<T>& aVector1, const vector3<T>& aVector2)
	{
		return (aVector1.x == aVector2.x && aVector1.y == aVector2.y && aVector1.z == aVector2.z);
	}

	template<class T>
	bool operator!=(const vector3<T>& aVector1, const vector3<T>& aVector2)
	{
		return (aVector1.x != aVector2.x || aVector1.y != aVector2.y || aVector1.z != aVector2.z);
	}
#pragma endregion

#pragma region Methods
	template<class T>
	T vector3<T>::Length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	template<class T>
	T vector3<T>::LengthSqr() const
	{
		return (x * x + y * y + z * z);
	}

	template<class T>
	T vector3<T>::Dot(const vector3<T>& aOtherVector) const
	{
		return (x * aOtherVector.x + y * aOtherVector.y + z * aOtherVector.z);
	}

	template<class T>
	vector3<T> vector3<T>::Cross(const vector3<T>& aOtherVector) const
	{
		return vector3<T>(y * aOtherVector.z - aOtherVector.y * z, z * aOtherVector.x - aOtherVector.z * x, x * aOtherVector.y - aOtherVector.x * y);
	}

	template<class T>
	void vector3<T>::Normalize()
	{
		assert(Length() > 0 && "Can't Normalize vector with Length 0.");
		T multiplicationValue = 1 / Length();
		x *= multiplicationValue;
		y *= multiplicationValue;
		z *= multiplicationValue;
	}

	template<class T>
	vector3<T> vector3<T>::GetNormalized() const
	{
		assert(Length() > 0 && "Can't Normalize vector with Length 0.");
		T multiplicationValue = 1 / Length();
		return vector3<T>(x * multiplicationValue, y * multiplicationValue, z * multiplicationValue);
	}
#pragma endregion
}