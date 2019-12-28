#pragma once

#include <cmath>

namespace CommonUtilities
{
	template <class T>
	class Vector2;

	template <class T>
	class Vector4;

	template <class T>
	class Vector3
	{
	public:
		#pragma warning(disable: 4201)
		union
		{
			struct
			{
				T x;
				T y;
				T z;
			};
			T pData[3];
		};
		#pragma warning(default: 4201)

		Vector3<T>();
		Vector3<T>(const T& aX, const T& aY, const T& aZ);
		Vector3<T>(T aData[3]);
		Vector3<T>(const Vector3<T>& aVector) = default;
		Vector3<T>(const Vector2<T>& aVector, const T& aZ = 0);
		Vector3<T>(const Vector4<T>& aVector);
		Vector3<T>& operator=(const Vector3<T>& aVector) = default;
		~Vector3<T>() = default;

		T Length2() const;
		T Length() const;

		Vector3<T> GetNormalized() const;
		void Normalize();

		T Dot(const Vector3<T>& aVector) const;
		
		Vector3<T> Cross(const Vector3<T>& aVector) const;

		static Vector3<T> Right;
		static Vector3<T> Up;
		static Vector3<T> Forward;
		static Vector3<T> Zero;
		static Vector3<T> One;
	};

	template <class T>
	typename Vector3<T> Vector3<T>::Right = { 1.f, 0.f, 0.f };
	template <class T>
	typename Vector3<T> Vector3<T>::Up = { 0.f, 1.f, 0.f };
	template <class T>
	typename Vector3<T> Vector3<T>::Forward = { 0.f, 0.f, 1.f };
	template <class T>
	typename Vector3<T> Vector3<T>::Zero = { 0.f, 0.f, 0.f };
	template <class T>
	typename Vector3<T> Vector3<T>::One = { 1.f, 1.f, 1.f };

	template <class T>
	Vector3<T>::Vector3()
	{
		x = static_cast<T>(0);
		y = static_cast<T>(0);
		z = static_cast<T>(0);
	}

	template <class T>
	Vector3<T>::Vector3(const T& aX, const T& aY, const T& aZ)

	{
		x = aX;
		y = aY;
		z = aZ;
	}

	template<class T>
	inline Vector3<T>::Vector3(T aData[3])
	{
		x = aData[0];
		y = aData[1];
		z = aData[2];
	}

	template<class T>
	inline Vector3<T>::Vector3(const Vector2<T>& aVector, const T & aZ)
	{
		x = aVector.x;
		y = aVector.y;
		z = aZ;
	}

	template<class T>
	inline Vector3<T>::Vector3(const Vector4<T>& aVector)
	{
		x = aVector.x;
		y = aVector.y;
		z = aVector.z;
	}

	template <class T>
	Vector3<T> operator+(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return Vector3<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z);
	}

	template <class T>
	Vector3<T> operator-(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return Vector3<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y, aVector0.z - aVector1.z);
	}

	template <class T>
	Vector3<T> operator-(const Vector3<T>& aVector)
	{
		return Vector3<T>(-aVector.x, -aVector.y, -aVector.z);
	}

	template <class T>
	Vector3<T> operator*(const Vector3<T>& aVector, const T& aScalar)
	{
		return Vector3<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar);
	}

	template <class T>
	Vector3<T> operator*(const T& aScalar, const Vector3<T>& aVector)
	{
		return Vector3<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar);
	}
	
	template <class T>
	Vector3<T> operator/(const Vector3<T>& aVector, const T& aScalar)
	{
		return Vector3<T>(aVector.x / aScalar, aVector.y / aScalar, aVector.z / aScalar);
	}

	template <class T>
	Vector3<T> operator+=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
		aVector0.z += aVector1.z;
		return aVector0;
	}

	template <class T>
	Vector3<T> operator-=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
		aVector0.z -= aVector1.z;
		return aVector0;
	}

	template <class T>
	bool operator==(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return aVector0.x == aVector1.x &&
			aVector0.y == aVector1.y &&
			aVector0.z == aVector1.z;
	}

	template <class T>
	Vector3<T> operator*=(Vector3<T>& aVector0, const T& aScalar)
	{
		aVector0.x *= aScalar;
		aVector0.y *= aScalar;
		aVector0.z *= aScalar;
		return aVector0;
	}

	template <class T>
	Vector3<T> operator/=(Vector3<T>& aVector0, const T& aScalar)
	{
		aVector0.x /= aScalar;
		aVector0.y /= aScalar;
		aVector0.z /= aScalar;
		return aVector0;
	}

	template <class T>
	T Vector3<T>::Length2() const
	{
		return x*x + y*y + z*z;
	}

	template <class T>
	T Vector3<T>::Length() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	template <class T>
	Vector3<T> Vector3<T>::GetNormalized() const
	{
		T length = Length2();
		if (length > static_cast<T>(0))
		{
			return *this / static_cast<T>(sqrt(length));
		}
		return *this;
	}

	template <class T>
	void Vector3<T>::Normalize()
	{
		T length = Length2();
		if (length > static_cast<T>(0))
		{
			*this /= static_cast<T>(sqrt(length));
		}
	}

	template <class T>
	T Vector3<T>::Dot(const Vector3<T>& aVector) const
	{
		return x*aVector.x + y*aVector.y + z*aVector.z;
	}

	template <class T>
	Vector3<T> Vector3<T>::Cross(const Vector3<T>& aVector) const
	{
		return Vector3<T>(
			(y * aVector.z) - (z * aVector.y),
			(z * aVector.x) - (x * aVector.z),
			(x * aVector.y) - (y * aVector.x)
		);
	}
}
