#pragma once
#include <cassert>

namespace math
{
	template<class T>
	class vector2
	{
	public:
		T x;
		T y;

		vector2<T>();
		vector2<T>(T aX, T aY);
		~vector2<T>() = default;

		T Length() const;
		T LengthSqr() const;
		T Dot(const vector2<T>& aOtherVector) const;
		void Normalize();
		vector2<T> GetNormalized() const;

		vector2<T>& operator=(const vector2<T>& aOtherVector) = default;
	};

#pragma region Constructors
	template<class T>
	vector2<T>::vector2()
	{
		x = {};
		y = {};
	}

	template<class T>
	vector2<T>::vector2(T aX, T aY)
	{
		x = aX;
		y = aY;
	}
#pragma endregion

#pragma region Operators
	template <class T>
	vector2<T> operator+(const vector2<T>& aVector1, const vector2<T>& aVector2)
	{
		return vector2<T>(aVector1.x + aVector2.x, aVector1.y + aVector2.y);
	}

	template<class T>
	vector2<T> operator-(const vector2<T>& aVector1, const vector2<T>& aVector2)
	{
		return vector2<T>(aVector1.x - aVector2.x, aVector1.y - aVector2.y);
	}

	template<class T>
	vector2<T> operator*(const vector2<T>& aVector, const T& aScalar)
	{
		return vector2<T>(aVector.x * aScalar, aVector.y * aScalar);
	}

	template<class T>
	vector2<T> operator/(const vector2<T>& aVector, const T& aScalar)
	{
		assert(aScalar > 0 && "Can't divide vector with 0.");
		T multiplicationValue = 1 / aScalar;
		return vector2<T>(aVector.x * multiplicationValue, aVector.y * multiplicationValue);
	}

	template<class T>
	void operator+=(vector2<T>& aVector1, const vector2<T>& aVector2)
	{
		aVector1.x = aVector1.x + aVector2.x;
		aVector1.y = aVector1.y + aVector2.y;
	}

	template<class T>
	void operator-=(vector2<T>& aVector1, const vector2<T>& aVector2)
	{
		aVector1.x = aVector1.x - aVector2.x;
		aVector1.y = aVector1.y - aVector2.y;
	}

	template<class T>
	void operator*=(vector2<T>& aVector, const T& aScalar)
	{
		aVector.x = aVector.x * aScalar;
		aVector.y = aVector.y * aScalar;
	}

	template<class T>
	void operator/=(vector2<T>& aVector, const T& aScalar)
	{
		assert(aScalar > 0 && "Can't divide vector with 0.");
		T multiplicationValue = 1 / aScalar;
		aVector.x = aVector.x * multiplicationValue;
		aVector.y = aVector.y * multiplicationValue;
	}

	template<class T>
	bool operator==(const vector2<T>& aVector1, const vector2<T>& aVector2)
	{
		return (aVector1.x == aVector2.x && aVector1.y == aVector2.y);
	}

	template<class T>
	bool operator!=(const vector2<T>& aVector1, const vector2<T>& aVector2)
	{
		return (aVector1.x != aVector2.x || aVector1.y != aVector2.y);
	}
#pragma endregion

#pragma region Methods
	template<class T>
	T vector2<T>::Length() const
	{
		return sqrt(x * x + y * y);
	}

	template<class T>
	T vector2<T>::LengthSqr() const
	{
		return (x * x + y * y);
	}

	template<class T>
	T vector2<T>::Dot(const vector2<T>& aOtherVector) const
	{
		return (x * aOtherVector.x + y * aOtherVector.y);
	}

	template<class T>
	void vector2<T>::Normalize()
	{
		assert(Length() > 0 && "Can't Normalize vector with Length 0.");
		T multiplicationValue = 1 / Length();
		x *= multiplicationValue;
		y *= multiplicationValue;
	}

	template<class T>
	vector2<T> vector2<T>::GetNormalized() const
	{
		assert(Length() > 0 && "Can't Normalize vector with Length 0.");
		T multiplicationValue = 1 / Length();
		return vector2<T>(x * multiplicationValue, y * multiplicationValue);
	}
#pragma endregion
}