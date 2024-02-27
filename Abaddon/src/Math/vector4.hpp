#pragma once
#include <cassert>

namespace math
{
	template<class T>
	class vector4
	{
	public:
		T x;
		T y;
		T z;
		T w;

		vector4<T>();
		vector4<T>(T aX, T aY, T aZ, T aW);
		~vector4<T>() = default;

		T Length() const;
		T LengthSqr() const;
		T Dot(const vector4<T>& aOtherVector) const;
		void Normalize();
		vector4<T> GetNormalized() const;

		vector4<T>& operator=(const vector4<T>& aOtherVector) = default;
	};

#pragma region Constructors
	template<class T>
	vector4<T>::vector4()
	{
		x = {};
		y = {};
		z = {};
		w = {};
	}

	template<class T>
	vector4<T>::vector4(T aX, T aY, T aZ, T aW)
	{
		x = aX;
		y = aY;
		z = aZ;
		w = aW;
	}
#pragma endregion

#pragma region Operators
	template <class T>
	vector4<T> operator+(const vector4<T>& aVector1, const vector4<T>& aVector2)
	{
		return vector4<T>(aVector1.x + aVector2.x, aVector1.y + aVector2.y, aVector1.z + aVector2.z, aVector1.w + aVector2.w);
	}

	template<class T>
	vector4<T> operator-(const vector4<T>& aVector1, const vector4<T>& aVector2)
	{
		return vector4<T>(aVector1.x - aVector2.x, aVector1.y - aVector2.y, aVector1.z - aVector2.z, aVector1.w - aVector2.w);
	}

	template<class T>
	vector4<T> operator*(const vector4<T>& aVector, const T& aScalar)
	{
		return vector4<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar, aVector.w * aScalar);
	}

	template<class T>
	vector4<T> operator/(const vector4<T>& aVector, const T& aScalar)
	{
		assert(aScalar > 0 && "Can't divide vector with 0.");
		T multiplicationValue = 1 / aScalar;
		return vector4<T>(aVector.x * multiplicationValue, aVector.y * multiplicationValue, aVector.z * multiplicationValue, aVector.w * multiplicationValue);
	}

	template<class T>
	void operator+=(vector4<T>& aVector1, const vector4<T>& aVector2)
	{
		aVector1.x = aVector1.x + aVector2.x;
		aVector1.y = aVector1.y + aVector2.y;
		aVector1.z = aVector1.z + aVector2.z;
		aVector1.w = aVector1.w + aVector2.w;
	}

	template<class T>
	void operator-=(vector4<T>& aVector1, const vector4<T>& aVector2)
	{
		aVector1.x = aVector1.x - aVector2.x;
		aVector1.y = aVector1.y - aVector2.y;
		aVector1.z = aVector1.z - aVector2.z;
		aVector1.w = aVector1.w - aVector2.w;
	}

	template<class T>
	void operator*=(vector4<T>& aVector, const T& aScalar)
	{
		aVector.x = aVector.x * aScalar;
		aVector.y = aVector.y * aScalar;
		aVector.z = aVector.z * aScalar;
		aVector.w = aVector.w * aScalar;
	}

	template<class T>
	void operator/=(vector4<T>& aVector, const T& aScalar)
	{
		assert(aScalar > 0 && "Can't divide vector with 0.");
		T multiplicationValue = 1 / aScalar;
		aVector.x = aVector.x * multiplicationValue;
		aVector.y = aVector.y * multiplicationValue;
		aVector.z = aVector.z * multiplicationValue;
		aVector.w = aVector.w * multiplicationValue;
	}

	template<class T>
	bool operator==(const vector4<T>& aVector1, const vector4<T>& aVector2)
	{
		return (aVector1.x == aVector2.x && aVector1.y == aVector2.y && aVector1.z == aVector2.z && aVector1.w == aVector2.w);
	}

	template<class T>
	bool operator!=(const vector4<T>& aVector1, const vector4<T>& aVector2)
	{
		return (aVector1.x != aVector2.x || aVector1.y != aVector2.y || aVector1.z != aVector2.z || aVector1.w != aVector2.w);
	}
#pragma endregion

#pragma region Methods
	template<class T>
	T vector4<T>::Length() const
	{
		return sqrt(x * x + y * y + z * z + w * w);
	}

	template<class T>
	T vector4<T>::LengthSqr() const
	{
		return (x * x + y * y + z * z + w * w);
	}

	template<class T>
	T vector4<T>::Dot(const vector4<T>& aOtherVector) const
	{
		return (x * aOtherVector.x + y * aOtherVector.y + z * aOtherVector.z + w * aOtherVector.w);
	}

	template<class T>
	void vector4<T>::Normalize()
	{
		assert(Length() > 0 && "Can't Normalize vector with Length 0.");
		T multiplicationValue = 1 / Length();
		x *= multiplicationValue;
		y *= multiplicationValue;
		z *= multiplicationValue;
		w *= multiplicationValue;
	}

	template<class T>
	vector4<T> vector4<T>::GetNormalized() const
	{
		assert(Length() > 0 && "Can't Normalize vector with Length 0.");
		T multiplicationValue = 1 / Length();
		return vector4<T>(x * multiplicationValue, y * multiplicationValue, z * multiplicationValue, w * multiplicationValue);
	}
#pragma endregion
}