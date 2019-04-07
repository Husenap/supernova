#pragma once

#include <cmath>

namespace CommonUtilities
{
	template <class T>
	class Vector4;

	template <class T>
	class Vector3;

	template <class T>
	class Vector2
	{
	public:
		#pragma warning(disable: 4201)
		union
		{
			struct
			{
				T x;
				T y;
			};
			T pData[2];
		};
		#pragma warning(default: 4201)


		Vector2<T>();
		Vector2<T>(const T& aX, const T& aY);
		Vector2<T>(T aData[2]);
		Vector2<T>(const Vector2<T>& aVector) = default;
		Vector2<T>(const Vector3<T>& aVector);
		Vector2<T>(const Vector4<T>& aVector);
		Vector2<T>& operator=(const Vector2<T>& aVector) = default;
		~Vector2<T>() = default;

		T Length2() const;
		T Length() const;

		Vector2<T> GetNormal() const;
		Vector2<T> GetNormalized() const;
		void Normalize();

		T Dot(const Vector2<T>& aVector) const;

		float GetAngle() const;
	};

	template <class T>
	Vector2<T>::Vector2()
	{
		x = static_cast<T>(0);
		y = static_cast<T>(0);
	}

	template <class T>
	Vector2<T>::Vector2(const T& aX, const T& aY)
	{
		x = aX;
		y = aY;
	}

	template<class T>
	inline Vector2<T>::Vector2(T aData[2])
	{
		x = aData[0];
		y = aData[1];
	}

	template<class T>
	inline Vector2<T>::Vector2(const Vector3<T>& aVector)
	{
		x = aVector.x;
		y = aVector.y;
	}

	template<class T>
	inline Vector2<T>::Vector2(const Vector4<T>& aVector)
	{
		x = aVector.x;
		y = aVector.y;
	}

	template <typename T>
	inline Vector2<T> operator+(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return Vector2<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y);
	}

	template <class T>
	inline Vector2<T> operator-(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return Vector2<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y);
	}

	template <class T>
	inline Vector2<T> operator-(const Vector2<T>& aVector)
	{
		return Vector2<T>(-aVector.x, -aVector.y);
	}

	template <class T>
	inline Vector2<T> operator*(const Vector2<T>& aVector, const T& aScalar)
	{
		return Vector2<T>(aVector.x * aScalar, aVector.y * aScalar);
	}

	template <class T>
	inline Vector2<T> operator*(const T& aScalar, const Vector2<T>& aVector)
	{
		return Vector2<T>(aVector.x * aScalar, aVector.y * aScalar);
	}
	
	template <class T>
	inline Vector2<T> operator/(const Vector2<T>& aVector, const T& aScalar)
	{
		return Vector2<T>(aVector.x / aScalar, aVector.y / aScalar);
	}

	template <class T>
	inline Vector2<T> operator+=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
		return aVector0;
	}

	template <class T>
	inline Vector2<T> operator-=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
		return aVector0;
	}

	template <class T>
	inline Vector2<T> operator*=(Vector2<T>& aVector0, const T& aScalar)
	{
		aVector0.x *= aScalar;
		aVector0.y *= aScalar;
		return aVector0;
	}

	template <class T>
	inline Vector2<T> operator/=(Vector2<T>& aVector0, const T& aScalar)
	{
		aVector0.x /= aScalar;
		aVector0.y /= aScalar;
		return aVector0;
	}

	template <class T>
	inline T Vector2<T>::Length2() const
	{
		return x*x + y*y;
	}

	template <class T>
	inline T Vector2<T>::Length() const
	{
		return sqrt(x*x + y*y);
	}

	template<class T>
	inline Vector2<T> Vector2<T>::GetNormal() const
	{
		return Vector2<T>(-y, x);
	}

	template <class T>
	inline Vector2<T> Vector2<T>::GetNormalized() const
	{
		T length = Length2();
		if (length > static_cast<T>(0))
		{
			return *this / static_cast<T>(sqrt(length));
		}
		return *this;
	}

	template <class T>
	inline void Vector2<T>::Normalize()
	{
		T length = Length2();
		if (length > static_cast<T>(0))
		{
			*this /= static_cast<T>(sqrt(length));
		}
	}

	template <class T>
	inline T Vector2<T>::Dot(const Vector2<T>& aVector) const
	{
		return x*aVector.x + y*aVector.y;
	}
	template<class T>
	inline float Vector2<T>::GetAngle() const
	{
		return std::atan2f(y, x);
	}
}
