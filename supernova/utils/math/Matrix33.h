#pragma once

#include <assert.h>
#include <math.h>

#include "Matrix44.h"
#include "Vector3.h"

namespace CommonUtilities
{
	template <typename T>
	class Matrix33
	{
	public:
		Matrix33<T>( T m11 = 1, T m12 = 0, T m13 = 0, T m21 = 0, T m22 = 1, T m23 = 0, T m31 = 0, T m32 = 0, T m33 = 1 );
		Matrix33<T>(const Matrix33<T>& aMatrix);
		Matrix33<T>(const Matrix44<T>& aMatrix);

		inline Matrix33<T>& operator=(const Matrix33<T>& aMatrix);

		inline T& operator[](const short& aIndex);
		inline const T& operator[](const short& aIndex) const;

		inline bool operator==(const Matrix33<T>& aMatrix);

		static Matrix33<T> CreateRotateAroundX(T aAngleInRadians);
		static Matrix33<T> CreateRotateAroundY(T aAngleInRadians);
		static Matrix33<T> CreateRotateAroundZ(T aAngleInRadians);
		static Matrix33<T> CreateRotateAround(const Vector3<T>& aAxis, float aAngle);
		static Matrix33<T> CreateYawPitchRoll(Vector3<T> aRotation);

		static Matrix33<T> Transpose(const Matrix33<T>& aMatrixToTranspose);

		#pragma warning(disable: 4201)
		union
		{
			T myMatrix[9];
			struct  
			{
				CommonUtilities::Vector3<T> myRightAxis;
				CommonUtilities::Vector3<T> myUpAxis;
				CommonUtilities::Vector3<T> myForwardAxis;
			};
		};
		#pragma warning(default: 4201)
	};

	template<typename T>
	inline Matrix33<T>::Matrix33(T m11, T m12, T m13, T m21, T m22, T m23, T m31, T m32, T m33)
	{
		myMatrix[0] = m11; myMatrix[1] = m12; myMatrix[2] = m13;
		myMatrix[3] = m21; myMatrix[4] = m22; myMatrix[5] = m23;
		myMatrix[6] = m31; myMatrix[7] = m32; myMatrix[8] = m33;
	}
	template<typename T>
	inline Matrix33<T>::Matrix33(const Matrix33<T>& aMatrix)
	{
		myMatrix[0] = aMatrix[0];	myMatrix[1] = aMatrix[1];	myMatrix[2] = aMatrix[2];
		myMatrix[3] = aMatrix[3];	myMatrix[4] = aMatrix[4];	myMatrix[5] = aMatrix[5];
		myMatrix[6] = aMatrix[6];	myMatrix[7] = aMatrix[7];	myMatrix[8] = aMatrix[8];
	}
	template<typename T>
	inline Matrix33<T>::Matrix33(const Matrix44<T>& aMatrix)
	{
		myMatrix[0] = aMatrix[0];	myMatrix[1] = aMatrix[1];	myMatrix[2] = aMatrix[2];
		myMatrix[3] = aMatrix[4];	myMatrix[4] = aMatrix[5];	myMatrix[5] = aMatrix[6];
		myMatrix[6] = aMatrix[8];	myMatrix[7] = aMatrix[9];	myMatrix[8] = aMatrix[10];
	}

	template<typename T>
	inline Matrix33<T>& Matrix33<T>::operator=(const Matrix33<T>& aMatrix)
	{
		myMatrix[0] = aMatrix[0];	myMatrix[1] = aMatrix[1];	myMatrix[2] = aMatrix[2];
		myMatrix[3] = aMatrix[3];	myMatrix[4] = aMatrix[4];	myMatrix[5] = aMatrix[5];
		myMatrix[6] = aMatrix[6];	myMatrix[7] = aMatrix[7];	myMatrix[8] = aMatrix[8];
		return *this;
	}

	template<typename T>
	inline T & Matrix33<T>::operator[](const short & aIndex)
	{
		assert(aIndex >= 0 && aIndex < 9 && "Index out of bounds");
		return myMatrix[aIndex];
	}
	template<typename T>
	inline const T & Matrix33<T>::operator[](const short & aIndex) const
	{
		assert(aIndex >= 0 && aIndex < 9 && "Index out of bounds");
		return myMatrix[aIndex];
	}

	template<typename T>
	inline Matrix33<T>& operator*=(Matrix33<T>& aMatrix, const T & aScalar)
	{
		aMatrix[0] *= aScalar; aMatrix[1] *= aScalar; aMatrix[2] *= aScalar;
		aMatrix[3] *= aScalar; aMatrix[4] *= aScalar; aMatrix[5] *= aScalar;
		aMatrix[6] *= aScalar; aMatrix[7] *= aScalar; aMatrix[8] *= aScalar;
		return aMatrix;
	}
	template<typename T>
	inline Matrix33<T> operator*(const Matrix33<T>& aMatrix, const T & aScalar)
	{
		return Matrix33<T>(
			aMatrix[0] * aScalar, aMatrix[1] * aScalar, aMatrix[2] * aScalar,
			aMatrix[3] * aScalar, aMatrix[4] * aScalar, aMatrix[5] * aScalar,
			aMatrix[6] * aScalar, aMatrix[7] * aScalar, aMatrix[8] * aScalar
		);
	}
	template<typename T>
	inline Matrix33<T> operator*(const T & aScalar, const Matrix33<T>& aMatrix)
	{
		return Matrix33<T>(
			aMatrix[0] * aScalar, aMatrix[1] * aScalar, aMatrix[2] * aScalar,
			aMatrix[3] * aScalar, aMatrix[4] * aScalar, aMatrix[5] * aScalar,
			aMatrix[6] * aScalar, aMatrix[7] * aScalar, aMatrix[8] * aScalar
		);
	}

	template<typename T>
	inline Matrix33<T>& operator/=(Matrix33<T>& aMatrix, const T & aScalar)
	{
		aMatrix[0] /= aScalar; aMatrix[1] /= aScalar; aMatrix[2] /= aScalar;
		aMatrix[3] /= aScalar; aMatrix[4] /= aScalar; aMatrix[5] /= aScalar;
		aMatrix[6] /= aScalar; aMatrix[7] /= aScalar; aMatrix[8] /= aScalar;
		return aMatrix;
	}
	template<typename T>
	inline Matrix33<T> operator/(const Matrix33<T>& aMatrix, const T & aScalar)
	{
		return Matrix33<T>(
			aMatrix[0] / aScalar, aMatrix[1] / aScalar, aMatrix[2] / aScalar,
			aMatrix[3] / aScalar, aMatrix[4] / aScalar, aMatrix[5] / aScalar,
			aMatrix[6] / aScalar, aMatrix[7] / aScalar, aMatrix[8] / aScalar
		);
	}

	template<typename T>
	inline Matrix33<T>& operator+=(Matrix33<T>& aMatrix0, const Matrix33<T>& aMatrix1)
	{
		aMatrix0[0] += aMatrix1[0];	aMatrix0[1] += aMatrix1[1];	aMatrix0[2] += aMatrix1[2];
		aMatrix0[3] += aMatrix1[3];	aMatrix0[4] += aMatrix1[4];	aMatrix0[5] += aMatrix1[5];
		aMatrix0[6] += aMatrix1[6];	aMatrix0[7] += aMatrix1[7];	aMatrix0[8] += aMatrix1[8];
		return aMatrix0;
	}
	template<typename T>
	inline Matrix33<T>& operator-=(Matrix33<T>& aMatrix0, const Matrix33<T>& aMatrix1)
	{
		aMatrix0[0] -= aMatrix1[0];	aMatrix0[1] -= aMatrix1[1];	aMatrix0[2] -= aMatrix1[2];
		aMatrix0[3] -= aMatrix1[3];	aMatrix0[4] -= aMatrix1[4];	aMatrix0[5] -= aMatrix1[5];
		aMatrix0[6] -= aMatrix1[6];	aMatrix0[7] -= aMatrix1[7];	aMatrix0[8] -= aMatrix1[8];
		return aMatrix0;
	}
	template<typename T>
	inline Matrix33<T> operator+(const Matrix33<T>& aMatrix0, const Matrix33<T>& aMatrix1)
	{
		return Matrix33<T>(
			aMatrix0[0] + aMatrix1[0], aMatrix0[1] + aMatrix1[1], aMatrix0[2] + aMatrix1[2],
			aMatrix0[3] + aMatrix1[3], aMatrix0[4] + aMatrix1[4], aMatrix0[5] + aMatrix1[5],
			aMatrix0[6] + aMatrix1[6], aMatrix0[7] + aMatrix1[7], aMatrix0[8] + aMatrix1[8]
		);
	}
	template<typename T>
	inline Matrix33<T> operator-(const Matrix33<T>& aMatrix0, const Matrix33<T>& aMatrix1)
	{
		return Matrix33<T>(
			aMatrix0[0] - aMatrix1[0], aMatrix0[1] - aMatrix1[1], aMatrix0[2] - aMatrix1[2],
			aMatrix0[3] - aMatrix1[3], aMatrix0[4] - aMatrix1[4], aMatrix0[5] - aMatrix1[5],
			aMatrix0[6] - aMatrix1[6], aMatrix0[7] - aMatrix1[7], aMatrix0[8] - aMatrix1[8]
		);
	}

	template<typename T>
	inline Matrix33<T>& operator*=(Matrix33<T>& aMatrix0, const Matrix33<T>& aMatrix1)
	{
		T m[9];
		m[0] = aMatrix0[0]; m[1] = aMatrix0[1]; m[2] = aMatrix0[2];
		m[3] = aMatrix0[3]; m[4] = aMatrix0[4]; m[5] = aMatrix0[5];
		m[6] = aMatrix0[6]; m[7] = aMatrix0[7]; m[8] = aMatrix0[8];

		aMatrix0[0] = (m[0] * aMatrix1[0]) + (m[1] * aMatrix1[3]) + (m[2] * aMatrix1[6]);
		aMatrix0[1] = (m[0] * aMatrix1[1]) + (m[1] * aMatrix1[4]) + (m[2] * aMatrix1[7]);
		aMatrix0[2] = (m[0] * aMatrix1[2]) + (m[1] * aMatrix1[5]) + (m[2] * aMatrix1[8]);

		aMatrix0[3] = (m[3] * aMatrix1[0]) + (m[4] * aMatrix1[3]) + (m[5] * aMatrix1[6]);
		aMatrix0[4] = (m[3] * aMatrix1[1]) + (m[4] * aMatrix1[4]) + (m[5] * aMatrix1[7]);
		aMatrix0[5] = (m[3] * aMatrix1[2]) + (m[4] * aMatrix1[5]) + (m[5] * aMatrix1[8]);

		aMatrix0[6] = (m[6] * aMatrix1[0]) + (m[7] * aMatrix1[3]) + (m[8] * aMatrix1[6]);
		aMatrix0[7] = (m[6] * aMatrix1[1]) + (m[7] * aMatrix1[4]) + (m[8] * aMatrix1[7]);
		aMatrix0[8] = (m[6] * aMatrix1[2]) + (m[7] * aMatrix1[5]) + (m[8] * aMatrix1[8]);

		return aMatrix0;
	}
	template<typename T>
	inline Matrix33<T> operator*(const Matrix33<T>& aMatrix0, const Matrix33<T>& aMatrix1)
	{
		return Matrix33<T>(
			(aMatrix0[0] * aMatrix1[0]) + (aMatrix0[1] * aMatrix1[3]) + (aMatrix0[2] * aMatrix1[6]),
			(aMatrix0[0] * aMatrix1[1]) + (aMatrix0[1] * aMatrix1[4]) + (aMatrix0[2] * aMatrix1[7]),
			(aMatrix0[0] * aMatrix1[2]) + (aMatrix0[1] * aMatrix1[5]) + (aMatrix0[2] * aMatrix1[8]),

			(aMatrix0[3] * aMatrix1[0]) + (aMatrix0[4] * aMatrix1[3]) + (aMatrix0[5] * aMatrix1[6]),
			(aMatrix0[3] * aMatrix1[1]) + (aMatrix0[4] * aMatrix1[4]) + (aMatrix0[5] * aMatrix1[7]),
			(aMatrix0[3] * aMatrix1[2]) + (aMatrix0[4] * aMatrix1[5]) + (aMatrix0[5] * aMatrix1[8]),

			(aMatrix0[6] * aMatrix1[0]) + (aMatrix0[7] * aMatrix1[3]) + (aMatrix0[8] * aMatrix1[6]),
			(aMatrix0[6] * aMatrix1[1]) + (aMatrix0[7] * aMatrix1[4]) + (aMatrix0[8] * aMatrix1[7]),
			(aMatrix0[6] * aMatrix1[2]) + (aMatrix0[7] * aMatrix1[5]) + (aMatrix0[8] * aMatrix1[8])
		);
	}

	template<typename T>
	inline Vector3<T>& operator*=(Vector3<T>& aVector, const Matrix33<T>& aMatrix)
	{
		T v[3];
		v[0] = aVector.x;
		v[1] = aVector.y;
		v[2] = aVector.z;
		
		aVector.x = (v[0] * aMatrix[0]) + (v[1] * aMatrix[3]) + (v[2] * aMatrix[6]);
		aVector.y = (v[0] * aMatrix[1]) + (v[1] * aMatrix[4]) + (v[2] * aMatrix[7]);
		aVector.z = (v[0] * aMatrix[2]) + (v[1] * aMatrix[5]) + (v[2] * aMatrix[8]);
		return aVector;
	}
	template<typename T>
	inline Vector3<T> operator*(const Vector3<T>& aVector, const Matrix33<T>& aMatrix)
	{
		return Vector3<T>(
			(aVector.x * aMatrix[0]) + (aVector.y * aMatrix[3]) + (aVector.z * aMatrix[6]),
			(aVector.x * aMatrix[1]) + (aVector.y * aMatrix[4]) + (aVector.z * aMatrix[7]),
			(aVector.x * aMatrix[2]) + (aVector.y * aMatrix[5]) + (aVector.z * aMatrix[8])
		);
	}

	template<typename T>
	inline bool Matrix33<T>::operator==(const Matrix33<T>& aMatrix)
	{
		return
			myMatrix[0] == aMatrix[0] && myMatrix[1] == aMatrix[1] && myMatrix[2] == aMatrix[2] &&
			myMatrix[3] == aMatrix[3] && myMatrix[4] == aMatrix[4] && myMatrix[5] == aMatrix[5] &&
			myMatrix[6] == aMatrix[6] && myMatrix[7] == aMatrix[7] && myMatrix[8] == aMatrix[8];
	}

	template<typename T>
	inline Matrix33<T> Matrix33<T>::CreateRotateAroundX(T aAngleInRadians)
	{
		T c = static_cast<T>(std::cos(static_cast<double>(aAngleInRadians)));
		T s = static_cast<T>(std::sin(static_cast<double>(aAngleInRadians)));
		return Matrix33<T>(
			1,	0,	0,
			0,	c,	s,
			0,	-s,	c
		);
	}
	template<typename T>
	inline Matrix33<T> Matrix33<T>::CreateRotateAroundY(T aAngleInRadians)
	{
		T c = static_cast<T>(std::cos(static_cast<double>(aAngleInRadians)));
		T s = static_cast<T>(std::sin(static_cast<double>(aAngleInRadians)));
		return Matrix33<T>(
			c,	0,	-s,
			0,	1,	0,
			s,	0,	c
		);
	}
	template<typename T>
	inline Matrix33<T> Matrix33<T>::CreateRotateAroundZ(T aAngleInRadians)
	{
		T c = static_cast<T>(std::cos(static_cast<double>(aAngleInRadians)));
		T s = static_cast<T>(std::sin(static_cast<double>(aAngleInRadians)));
		return Matrix33<T>(
			c,	s,	0,
			-s,	c,	0,
			0,	0,	1
		);
	}

	template<typename T>
	inline Matrix33<T> Matrix33<T>::CreateRotateAround(const Vector3<T>& aAxis, float aAngle)
	{
		const Vector3<T>& axis = aAxis.GetNormalized();
		float s = -std::sin(aAngle);
		float c = std::cos(aAngle);
		float oc = 1.f - c;
		return Matrix33<T>(
			oc * axis.x * axis.x + c,			oc * axis.x * axis.y - axis.z * s,	oc * axis.z * axis.x + axis.y * s,
			oc * axis.x * axis.y + axis.z * s,	oc * axis.y * axis.y + c,			oc * axis.y * axis.z - axis.x * s,
			oc * axis.z * axis.x - axis.y * s,	oc * axis.y * axis.z + axis.x * s,	oc * axis.z * axis.z + c
		);
	}

	template<typename T>
	inline Matrix33<T> Matrix33<T>::CreateYawPitchRoll(Vector3<T> aRotation)
	{
		float cy = std::cos(aRotation.y);
		float sy = std::sin(aRotation.y);
		float cx = std::cos(aRotation.x);
		float sx = std::sin(aRotation.x);
		float cz = std::cos(aRotation.z);
		float sz = std::sin(aRotation.z);
		
		return Matrix33<T>(
			(cy * cz),
			(cy * sz),
			(-sy),
			
			(sx * sy * cz + cx * -sz),
			(sx * sy * sz + cx * cz),
			(sx * cy),
			
			(cx * sy * cz + -sx * -sz),
			(cx * sy * sz + -sx * cz),
			(cx * cy)
		);
	}

	template<typename T>
	inline Matrix33<T> Matrix33<T>::Transpose(const Matrix33<T>& aMatrixToTranspose)
	{
		return Matrix33<T>(
			aMatrixToTranspose[0], aMatrixToTranspose[3], aMatrixToTranspose[6],
			aMatrixToTranspose[1], aMatrixToTranspose[4], aMatrixToTranspose[7],
			aMatrixToTranspose[2], aMatrixToTranspose[5], aMatrixToTranspose[8]
		);
	}

}
