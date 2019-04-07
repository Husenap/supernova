#pragma once

#include <cmath>

namespace CommonUtilities
{
	template <class T>
	class Vector3;

	template <class T>
	class Vector4
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
				T w;
			};
			T pData[4];
		};
		#pragma warning(default: 4201)

		Vector4<T>();
		Vector4<T>(const T& aX, const T& aY, const T& aZ, const T& aW);
		Vector4<T>(T aData[4]);
		Vector4<T>(const Vector4<T>& aVector) = default;
		Vector4<T>(const Vector3<T>& aVector, const T& aW = 0);
		Vector4<T>& operator=(const Vector4<T>& aVector) = default;
		~Vector4<T>() = default;

		T Length2() const;
		T Length() const;

		Vector4<T> GetNormalized() const;
		void Normalize();

		T Dot(const Vector4<T>& aVector) const;
	};

	template <class T>
	Vector4<T>::Vector4()
	{
		x = static_cast<T>(0);
		y = static_cast<T>(0);
		z = static_cast<T>(0);
		w = static_cast<T>(0);
	}

	template <class T>
	Vector4<T>::Vector4(const T& aX, const T& aY, const T& aZ, const T& aW)
	{
		x = aX;
		y = aY;
		z = aZ;
		w = aW;
	}

	template<class T>
	inline Vector4<T>::Vector4(T aData[4])
	{
		x = aData[0];
		y = aData[1];
		z = aData[2];
		w = aData[3];
	}

	template<class T>
	inline Vector4<T>::Vector4(const Vector3<T>& aVector, const T & aW)
	{
		x = aVector.x;
		y = aVector.y;
		z = aVector.z;
		w = aW;
	}

	template <class T>
	Vector4<T> operator+(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		return Vector4<T>(
			aVector0.x + aVector1.x,
			aVector0.y + aVector1.y,
			aVector0.z + aVector1.z,
			aVector0.w + aVector1.w
		);
	}

	template <class T>
	Vector4<T> operator-(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		return Vector4<T>(
			aVector0.x - aVector1.x,
			aVector0.y - aVector1.y,
			aVector0.z - aVector1.z,
			aVector0.w - aVector1.w
		);
	}

	template <class T>
	Vector4<T> operator-(const Vector4<T>& aVector)
	{
		return Vector4<T>(-aVector.x, -aVector.y, -aVector.z, -aVector.w);
	}

	template <class T>
	Vector4<T> operator*(const Vector4<T>& aVector, const T& aScalar)
	{
		return Vector4<T>(
			aVector.x * aScalar,
			aVector.y * aScalar,
			aVector.z * aScalar,
			aVector.w * aScalar
		);
	}

	template <class T>
	Vector4<T> operator*(const T& aScalar, const Vector4<T>& aVector)
	{
		return Vector4<T>(
			aVector.x * aScalar,
			aVector.y * aScalar,
			aVector.z * aScalar,
			aVector.w * aScalar
		);
	}
	
	template <class T>
	Vector4<T> operator/(const Vector4<T>& aVector, const T& aScalar)
	{
		return Vector4<T>(
			aVector.x / aScalar,
			aVector.y / aScalar,
			aVector.z / aScalar,
			aVector.w / aScalar
		);
	}

	template <class T>
	void operator+=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
		aVector0.z += aVector1.z;
		aVector0.w += aVector1.w;
	}

	template <class T>
	Vector4<T> operator-=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
		aVector0.z -= aVector1.z;
		aVector0.w -= aVector1.w;
		return aVector0;
	}

	template <class T>
	Vector4<T> operator*=(Vector4<T>& aVector0, const T& aScalar)
	{
		aVector0.x *= aScalar;
		aVector0.y *= aScalar;
		aVector0.z *= aScalar;
		aVector0.w *= aScalar;
		return aVector0;
	}

	template <class T>
	Vector4<T> operator/=(Vector4<T>& aVector0, const T& aScalar)
	{
		aVector0.x /= aScalar;
		aVector0.y /= aScalar;
		aVector0.z /= aScalar;
		aVector0.w /= aScalar;
		return aVector0;
	}

	template <class T>
	T Vector4<T>::Length2() const
	{
		return x*x + y*y + z*z + w*w;
	}

	template <class T>
	T Vector4<T>::Length() const
	{
		return sqrt(x*x + y*y + z*z + w*w);
	}

	template <class T>
	Vector4<T> Vector4<T>::GetNormalized() const
	{
		T length = Length2();
		if (length > static_cast<T>(0))
		{
			return *this / static_cast<T>(sqrt(length));
		}
		return *this;
	}

	template <class T>
	void Vector4<T>::Normalize()
	{
		T length = Length2();
		if (length > static_cast<T>(0))
		{
			*this /= static_cast<T>(sqrt(length));
		}
	}

	template <class T>
	T Vector4<T>::Dot(const Vector4<T>& aVector) const
	{
		return x*aVector.x + y*aVector.y + z*aVector.z + w*aVector.w;
	}
}