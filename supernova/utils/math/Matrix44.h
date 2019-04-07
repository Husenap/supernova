#pragma once

/*
*	Index list 3x3
*	0	1	2
*	3	4	5
*	6	7	8
*
*	Index list 4x4
*	0	1	2	3
*	4	5	6	7
*	8	9	10	11
*	12	13	14	15
*/

#include <xmmintrin.h>
#include <assert.h>
#include <math.h>

#include "Vector4.h"

namespace CommonUtilities
{
	template <typename T>
	class Matrix44
	{
	public:
		Matrix44<T>(
			T m11 = 1, T m12 = 0, T m13 = 0, T m14 = 0,
			T m21 = 0, T m22 = 1, T m23 = 0, T m24 = 0,
			T m31 = 0, T m32 = 0, T m33 = 1, T m34 = 0,
			T m41 = 0, T m42 = 0, T m43 = 0, T m44 = 1);
		Matrix44<T>(const Matrix44<T>& aMatrix);
		Matrix44<T>(const Vector3<T>& aRight, const Vector3<T>& aUp, const Vector3<T>& aForward, const Vector3<T>& aPosition);

		inline Matrix44<T>& operator=(const Matrix44<T>& aMatrix);

		inline T& operator[](const short& aIndex);
		inline const T& operator[](const short& aIndex) const;

		inline Matrix44<T>& operator*=(const T& aScalar);
		inline Matrix44<T> operator*(const T& aScalar) const;
		inline Matrix44<T>& operator/=(const T& aScalar);
		inline Matrix44<T> operator/(const T& aScalar) const;

		inline Matrix44<T>& operator+=(const Matrix44<T>& aMatrix);
		inline Matrix44<T>& operator-=(const Matrix44<T>& aMatrix);
		inline Matrix44<T> operator+(const Matrix44<T>& aMatrix);
		inline Matrix44<T> operator-(const Matrix44<T>& aMatrix);

		inline Matrix44<T> operator*(const Matrix44<T>& aMatrix);
		inline Matrix44<T>& operator*=(const Matrix44<T>& aMatrix);

		inline bool operator==(const Matrix44<T>& aMatrix);

		static Matrix44<T> CreateRotateAroundX(T aAngleInRadians);
		static Matrix44<T> CreateRotateAroundY(T aAngleInRadians);
		static Matrix44<T> CreateRotateAroundZ(T aAngleInRadians);
		static Matrix44<T> CreateRotateAround(const Vector3<T>& aAxis, float aAngle);
		static Matrix44<T> CreateYawPitchRoll(const Vector3<T>& aRotation);
		static Vector3<T> GetRotation(const Matrix44<T>& aMatrix);

		static Matrix44<T> Transpose(const Matrix44<T>& aMatrixToTranspose);
		static Matrix44<T> Inverse(const Matrix44<T>& aMatrixToInverse);

		Matrix44<T> GetFastInverse();

		#pragma warning(disable: 4201)
		union
		{
			struct  
			{
				T myMatrix[16];
			};
			struct
			{
				__m128 myM1;
				__m128 myM2;
				__m128 myM3;
				__m128 myM4;
			};
			struct  
			{
				CommonUtilities::Vector3<T> myRightAxis; T myRightW;
				CommonUtilities::Vector3<T> myUpAxis; T myUpW;
				CommonUtilities::Vector3<T> myForwardAxis; T myForwardW;
				CommonUtilities::Vector3<T> myPosition; T myW;
			};
		};
		#pragma warning(default: 4201)
	};

	template<typename T>
	inline Matrix44<T>::Matrix44(T m11, T m12, T m13, T m14, T m21, T m22, T m23, T m24, T m31, T m32, T m33, T m34, T m41, T m42, T m43, T m44)
	{
		myMatrix[0] = m11;	myMatrix[1] = m12;	myMatrix[2] = m13;	myMatrix[3] = m14;
		myMatrix[4] = m21;	myMatrix[5] = m22;	myMatrix[6] = m23;	myMatrix[7] = m24;
		myMatrix[8] = m31;	myMatrix[9] = m32;	myMatrix[10] = m33;myMatrix[11] = m34;
		myMatrix[12] = m41;myMatrix[13] = m42;myMatrix[14] = m43;myMatrix[15] = m44;
	}
	template<typename T>
	inline Matrix44<T>::Matrix44(const Matrix44<T>& aMatrix)
	{
		myMatrix[0] = aMatrix[0];	myMatrix[1] = aMatrix[1];	myMatrix[2] = aMatrix[2];	myMatrix[3] = aMatrix[3];
		myMatrix[4] = aMatrix[4];	myMatrix[5] = aMatrix[5];	myMatrix[6] = aMatrix[6];	myMatrix[7] = aMatrix[7];
		myMatrix[8] = aMatrix[8];	myMatrix[9] = aMatrix[9];	myMatrix[10] = aMatrix[10];myMatrix[11] = aMatrix[11];
		myMatrix[12] = aMatrix[12];myMatrix[13] = aMatrix[13];myMatrix[14] = aMatrix[14];myMatrix[15] = aMatrix[15];
	}

	template<typename T>
	inline Matrix44<T>::Matrix44(const Vector3<T>& aRight, const Vector3<T>& aUp, const Vector3<T>& aForward, const Vector3<T>& aPosition)
	{
		myRightAxis = aRight;
		myRightW = 0.f;
		myUpAxis = aUp;
		myUpW = 0.f;
		myForwardAxis = aForward;
		myForwardW = 0.f;
		myPosition = aPosition;
		myW = 1.f;
	}

	template<typename T>
	inline Matrix44<T>& Matrix44<T>::operator=(const Matrix44<T>& aMatrix)
	{
		myMatrix[0] = aMatrix[0];	myMatrix[1] = aMatrix[1];	myMatrix[2] = aMatrix[2];	myMatrix[3] = aMatrix[3];
		myMatrix[4] = aMatrix[4];	myMatrix[5] = aMatrix[5];	myMatrix[6] = aMatrix[6];	myMatrix[7] = aMatrix[7];
		myMatrix[8] = aMatrix[8];	myMatrix[9] = aMatrix[9];	myMatrix[10] = aMatrix[10];myMatrix[11] = aMatrix[11];
		myMatrix[12] = aMatrix[12];myMatrix[13] = aMatrix[13];myMatrix[14] = aMatrix[14];myMatrix[15] = aMatrix[15];
		return *this;
	}

	template<typename T>
	inline T & Matrix44<T>::operator[](const short & aIndex)
	{
		assert(aIndex >= 0 && aIndex < 16 && "Index out of bounds");
		return myMatrix[aIndex];
	}
	template<typename T>
	inline const T & Matrix44<T>::operator[](const short & aIndex) const
	{
		assert(aIndex >= 0 && aIndex < 16 && "Index out of bounds");
		return myMatrix[aIndex];
	}

	template<typename T>
	inline Matrix44<T>& Matrix44<T>::operator*=(const T & aScalar)
	{
		myMatrix[0] *= aScalar;	myMatrix[1] *= aScalar;	myMatrix[2] *= aScalar;	myMatrix[3] *= aScalar;
		myMatrix[4] *= aScalar;	myMatrix[5] *= aScalar;	myMatrix[6] *= aScalar;	myMatrix[7] *= aScalar;
		myMatrix[8] *= aScalar;	myMatrix[9] *= aScalar;	myMatrix[10] *= aScalar;	myMatrix[11] *= aScalar;
		myMatrix[12] *= aScalar;	myMatrix[13] *= aScalar;	myMatrix[14] *= aScalar;	myMatrix[15] *= aScalar;
		return *this;
	}
	template<typename T>
	inline Matrix44<T> Matrix44<T>::operator*(const T & aScalar) const
	{
		return Matrix44<T>(*this).operator*(aScalar);
	}
	template<typename T>
	inline Matrix44<T>& Matrix44<T>::operator/=(const T & aScalar)
	{
		myMatrix[0] /= aScalar;	myMatrix[1] /= aScalar;	myMatrix[2] /= aScalar;	myMatrix[3] /= aScalar;
		myMatrix[4] /= aScalar;	myMatrix[5] /= aScalar;	myMatrix[6] /= aScalar;	myMatrix[7] /= aScalar;
		myMatrix[8] /= aScalar;	myMatrix[9] /= aScalar;	myMatrix[10] /= aScalar;	myMatrix[11] /= aScalar;
		myMatrix[12] /= aScalar;	myMatrix[13] /= aScalar;	myMatrix[14] /= aScalar;	myMatrix[15] /= aScalar;
		return *this;
	}
	template<typename T>
	inline Matrix44<T> Matrix44<T>::operator/(const T & aScalar) const
	{
		return Matrix44<T>(*this).operator/=(aScalar);
	}

	template<typename T>
	inline Matrix44<T>& Matrix44<T>::operator+=(const Matrix44<T>& aMatrix)
	{
		myMatrix[0] += aMatrix[0]; myMatrix[1] += aMatrix[1]; myMatrix[2] += aMatrix[2]; myMatrix[3] += aMatrix[3];
		myMatrix[4] += aMatrix[4]; myMatrix[5] += aMatrix[5]; myMatrix[6] += aMatrix[6]; myMatrix[7] += aMatrix[7];
		myMatrix[8] += aMatrix[8]; myMatrix[9] += aMatrix[9]; myMatrix[10] += aMatrix[10]; myMatrix[11] += aMatrix[11];
		myMatrix[12] += aMatrix[12]; myMatrix[13] += aMatrix[13]; myMatrix[14] += aMatrix[14]; myMatrix[15] += aMatrix[15];
		return *this;
	}
	template<typename T>
	inline Matrix44<T>& Matrix44<T>::operator-=(const Matrix44<T>& aMatrix)
	{
		myMatrix[0] -= aMatrix[0]; myMatrix[1] -= aMatrix[1]; myMatrix[2] -= aMatrix[2]; myMatrix[3] -= aMatrix[3];
		myMatrix[4] -= aMatrix[4]; myMatrix[5] -= aMatrix[5]; myMatrix[6] -= aMatrix[6]; myMatrix[7] -= aMatrix[7];
		myMatrix[8] -= aMatrix[8]; myMatrix[9] -= aMatrix[9]; myMatrix[10] -= aMatrix[10]; myMatrix[11] -= aMatrix[11];
		myMatrix[12] -= aMatrix[12]; myMatrix[13] -= aMatrix[13]; myMatrix[14] -= aMatrix[14]; myMatrix[15] -= aMatrix[15];
		return *this;
	}
	template<typename T>
	inline Matrix44<T> Matrix44<T>::operator+(const Matrix44<T>& aMatrix)
	{
		return Matrix44<T>(*this).operator+=(aMatrix);
	}
	template<typename T>
	inline Matrix44<T> Matrix44<T>::operator-(const Matrix44<T>& aMatrix)
	{
		return Matrix44<T>(*this).operator-=(aMatrix);
	}

	template<typename T>
	inline Matrix44<T> Matrix44<T>::operator*(const Matrix44<T>& aMatrix)
	{
		return Matrix44<T>(*this).operator*=(aMatrix);
	}
	template<typename T>
	inline Matrix44<T>& Matrix44<T>::operator*=(const Matrix44<T>& aMatrix)
	{
		const __m128 a = aMatrix.myM1;
		const __m128 b = aMatrix.myM2;
		const __m128 c = aMatrix.myM3;
		const __m128 d = aMatrix.myM4;

		__m128 t1, t2;

		t1 = _mm_set1_ps(myMatrix[0]);
		t2 = _mm_mul_ps(a, t1);
		t1 = _mm_set1_ps(myMatrix[1]);
		t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
		t1 = _mm_set1_ps(myMatrix[2]);
		t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
		t1 = _mm_set1_ps(myMatrix[3]);
		t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);

		_mm_store_ps(&myMatrix[0], t2);

		t1 = _mm_set1_ps(myMatrix[4]);
		t2 = _mm_mul_ps(a, t1);
		t1 = _mm_set1_ps(myMatrix[5]);
		t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
		t1 = _mm_set1_ps(myMatrix[6]);
		t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
		t1 = _mm_set1_ps(myMatrix[7]);
		t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);

		_mm_store_ps(&myMatrix[4], t2);

		t1 = _mm_set1_ps(myMatrix[8]);
		t2 = _mm_mul_ps(a, t1);
		t1 = _mm_set1_ps(myMatrix[9]);
		t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
		t1 = _mm_set1_ps(myMatrix[10]);
		t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
		t1 = _mm_set1_ps(myMatrix[11]);
		t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);

		_mm_store_ps(&myMatrix[8], t2);

		t1 = _mm_set1_ps(myMatrix[12]);
		t2 = _mm_mul_ps(a, t1);
		t1 = _mm_set1_ps(myMatrix[13]);
		t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
		t1 = _mm_set1_ps(myMatrix[14]);
		t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
		t1 = _mm_set1_ps(myMatrix[15]);
		t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);

		_mm_store_ps(&myMatrix[12], t2);

		return *this;
	}

	template<typename T>
	inline Vector4<T>& operator*=(Vector4<T>& aVector, const Matrix44<T>& aMatrix)
	{
		T v[4];
		v[0] = aVector.x;
		v[1] = aVector.y;
		v[2] = aVector.z;
		v[3] = aVector.w;

		aVector.x = (v[0] * aMatrix[0]) + (v[1] * aMatrix[4]) + (v[2] * aMatrix[8]) + (v[3] * aMatrix[12]);
		aVector.y = (v[0] * aMatrix[1]) + (v[1] * aMatrix[5]) + (v[2] * aMatrix[9]) + (v[3] * aMatrix[13]);
		aVector.z = (v[0] * aMatrix[2]) + (v[1] * aMatrix[6]) + (v[2] * aMatrix[10]) + (v[3] * aMatrix[14]);
		aVector.w = (v[0] * aMatrix[3]) + (v[1] * aMatrix[7]) + (v[2] * aMatrix[11]) + (v[3] * aMatrix[15]);
		return aVector;
	}
	template<typename T>
	inline Vector4<T> operator*(const Vector4<T>& aVector, const Matrix44<T>& aMatrix)
	{
		return Vector4<T>(
			(aVector.x * aMatrix[0]) + (aVector.y * aMatrix[4]) + (aVector.z * aMatrix[8]) + (aVector.w * aMatrix[12]),
			(aVector.x * aMatrix[1]) + (aVector.y * aMatrix[5]) + (aVector.z * aMatrix[9]) + (aVector.w * aMatrix[13]),
			(aVector.x * aMatrix[2]) + (aVector.y * aMatrix[6]) + (aVector.z * aMatrix[10]) + (aVector.w * aMatrix[14]),
			(aVector.x * aMatrix[3]) + (aVector.y * aMatrix[7]) + (aVector.z * aMatrix[11]) + (aVector.w * aMatrix[15])
		);
	}

	template<typename T>
	inline bool Matrix44<T>::operator==(const Matrix44<T>& aMatrix)
	{
		return
			myMatrix[0] == aMatrix[0] && myMatrix[1] == aMatrix[1] && myMatrix[2] == aMatrix[2] && myMatrix[3] == aMatrix[3] &&
			myMatrix[4] == aMatrix[4] && myMatrix[5] == aMatrix[5] && myMatrix[6] == aMatrix[6] && myMatrix[7] == aMatrix[7] &&
			myMatrix[8] == aMatrix[8] && myMatrix[9] == aMatrix[9] && myMatrix[10] == aMatrix[10] && myMatrix[11] == aMatrix[11] &&
			myMatrix[12] == aMatrix[12] && myMatrix[13] == aMatrix[13] && myMatrix[14] == aMatrix[14] && myMatrix[15] == aMatrix[15];
	}

	template<typename T>
	inline Matrix44<T> Matrix44<T>::CreateRotateAroundX(T aAngleInRadians)
	{
		T c = static_cast<T>(std::cos(static_cast<double>(aAngleInRadians)));
		T s = static_cast<T>(std::sin(static_cast<double>(aAngleInRadians)));
		return Matrix44<T>(
			1,	0,	0,	0,
			0,	c,	s,	0,
			0,	-s,	c,	0,
			0,	0,	0,	1
		);
	}
	template<typename T>
	inline Matrix44<T> Matrix44<T>::CreateRotateAroundY(T aAngleInRadians)
	{
		T c = static_cast<T>(std::cos(static_cast<double>(aAngleInRadians)));
		T s = static_cast<T>(std::sin(static_cast<double>(aAngleInRadians)));
		return Matrix44<T>(
			c,	0,	-s,	0,
			0,	1,	0,	0,
			s,	0,	c,	0,
			0,	0,	0,	1
		);
	}
	template<typename T>
	inline Matrix44<T> Matrix44<T>::CreateRotateAroundZ(T aAngleInRadians)
	{
		T c = static_cast<T>(std::cos(static_cast<double>(aAngleInRadians)));
		T s = static_cast<T>(std::sin(static_cast<double>(aAngleInRadians)));
		return Matrix44<T>(
			c,	s,	0,	0,
			-s,	c,	0,	0,
			0,	0,	1,	0,
			0,	0,	0,	1
		);
	}

	template<typename T>
	inline Matrix44<T> Matrix44<T>::CreateRotateAround(const Vector3<T>& aAxis, float aAngle)
	{
		const Vector3<T>& axis = aAxis.GetNormalized();
		float s = -std::sin(aAngle);
		float c = std::cos(aAngle);
		float oc = 1.f - c;
		return Matrix44<T>(
			oc * axis.x * axis.x + c,			oc * axis.x * axis.y - axis.z * s,	oc * axis.z * axis.x + axis.y * s,	0,
			oc * axis.x * axis.y + axis.z * s,	oc * axis.y * axis.y + c,			oc * axis.y * axis.z - axis.x * s,	0,
			oc * axis.z * axis.x - axis.y * s,	oc * axis.y * axis.z + axis.x * s,	oc * axis.z * axis.z + c,			0,
			0,									0,									0,									1
		);
	}

	template<typename T>
	inline Matrix44<T> Matrix44<T>::CreateYawPitchRoll(const Vector3<T>& aRotation)
	{
		Matrix44<T> m;

		float cy = std::cos(aRotation.y);
		float sy = std::sin(aRotation.y);
		float cx = std::cos(aRotation.x);
		float sx = std::sin(aRotation.x);
		float cz = std::cos(aRotation.z);
		float sz = std::sin(aRotation.z);
		
		m[0] = (cy * cz);
		m[1] = (cy * sz);
		m[2] = (-sy);
		
		m[4] = (sx * sy * cz + cx * -sz);
		m[5] = (sx * sy * sz + cx * cz);
		m[6] = (sx * cy);
		
		m[8] = (cx * sy * cz + -sx * -sz);
		m[9] = (cx * sy * sz + -sx * cz);
		m[10] = (cx * cy);
		
		return m;
	}

	template<typename T>
	inline Vector3<T> Matrix44<T>::GetRotation(const Matrix44<T>& aMatrix)
	{
		Vector3<T> v;

		if (aMatrix[0] == 1.0f)
		{
			v.x = atan2f(aMatrix[2], aMatrix[11]);
			v.y = 0;
			v.z = 0;

		}
		else if (aMatrix[0] == -1.0f)
		{
			v.x = atan2f(aMatrix[2], aMatrix[11]);
			v.y = 0;
			v.z = 0;
		}
		else
		{
			v.x = atan2(-aMatrix[8], aMatrix[0]);
			v.y = asin(aMatrix[4]);
			v.z = atan2(-aMatrix[6], aMatrix[5]);
		}

		return v;
	}
	template<typename T>
	inline Matrix44<T> Matrix44<T>::Transpose(const Matrix44<T>& aMatrixToTranspose)
	{
		return Matrix44<T>(
			aMatrixToTranspose[0],	aMatrixToTranspose[4],	aMatrixToTranspose[8],	aMatrixToTranspose[12],
			aMatrixToTranspose[1],	aMatrixToTranspose[5],	aMatrixToTranspose[9],	aMatrixToTranspose[13],
			aMatrixToTranspose[2],	aMatrixToTranspose[6],	aMatrixToTranspose[10],	aMatrixToTranspose[14],
			aMatrixToTranspose[3],	aMatrixToTranspose[7],	aMatrixToTranspose[11],	aMatrixToTranspose[15]
		);
	}

	template<typename T>
	inline Matrix44<T> Matrix44<T>::GetFastInverse()
	{
		return Matrix44<T>(
			//Rotation
			myMatrix[0],	myMatrix[4],	myMatrix[8],	myMatrix[3],
			myMatrix[1],	myMatrix[5],	myMatrix[9],	myMatrix[7],
			myMatrix[2],	myMatrix[6],	myMatrix[10],	myMatrix[11],

			//Translation
			(-myMatrix[12]*myMatrix[0])+(-myMatrix[13]*myMatrix[1])+(-myMatrix[14]*myMatrix[2]),
			(-myMatrix[12]*myMatrix[4])+(-myMatrix[13]*myMatrix[5])+(-myMatrix[14]*myMatrix[6]),
			(-myMatrix[12]*myMatrix[8])+(-myMatrix[13]*myMatrix[9])+(-myMatrix[14]*myMatrix[10]),

			//W	
			myMatrix[15]
		);
	}

	template<class T>
	inline Matrix44<T> Matrix44<T>::Inverse(const Matrix44<T>& aMatrixToInverse)
	{
		T inv[16];

		inv[0] = aMatrixToInverse[5] * aMatrixToInverse[10] * aMatrixToInverse[15] -
			aMatrixToInverse[5] * aMatrixToInverse[11] * aMatrixToInverse[14] -
			aMatrixToInverse[9] * aMatrixToInverse[6] * aMatrixToInverse[15] +
			aMatrixToInverse[9] * aMatrixToInverse[7] * aMatrixToInverse[14] +
			aMatrixToInverse[13] * aMatrixToInverse[6] * aMatrixToInverse[11] -
			aMatrixToInverse[13] * aMatrixToInverse[7] * aMatrixToInverse[10];

		inv[4] = -aMatrixToInverse[4] * aMatrixToInverse[10] * aMatrixToInverse[15] +
			aMatrixToInverse[4] * aMatrixToInverse[11] * aMatrixToInverse[14] +
			aMatrixToInverse[8] * aMatrixToInverse[6] * aMatrixToInverse[15] -
			aMatrixToInverse[8] * aMatrixToInverse[7] * aMatrixToInverse[14] -
			aMatrixToInverse[12] * aMatrixToInverse[6] * aMatrixToInverse[11] +
			aMatrixToInverse[12] * aMatrixToInverse[7] * aMatrixToInverse[10];

		inv[8] = aMatrixToInverse[4] * aMatrixToInverse[9] * aMatrixToInverse[15] -
			aMatrixToInverse[4] * aMatrixToInverse[11] * aMatrixToInverse[13] -
			aMatrixToInverse[8] * aMatrixToInverse[5] * aMatrixToInverse[15] +
			aMatrixToInverse[8] * aMatrixToInverse[7] * aMatrixToInverse[13] +
			aMatrixToInverse[12] * aMatrixToInverse[5] * aMatrixToInverse[11] -
			aMatrixToInverse[12] * aMatrixToInverse[7] * aMatrixToInverse[9];

		inv[12] = -aMatrixToInverse[4] * aMatrixToInverse[9] * aMatrixToInverse[14] +
			aMatrixToInverse[4] * aMatrixToInverse[10] * aMatrixToInverse[13] +
			aMatrixToInverse[8] * aMatrixToInverse[5] * aMatrixToInverse[14] -
			aMatrixToInverse[8] * aMatrixToInverse[6] * aMatrixToInverse[13] -
			aMatrixToInverse[12] * aMatrixToInverse[5] * aMatrixToInverse[10] +
			aMatrixToInverse[12] * aMatrixToInverse[6] * aMatrixToInverse[9];

		inv[1] = -aMatrixToInverse[1] * aMatrixToInverse[10] * aMatrixToInverse[15] +
			aMatrixToInverse[1] * aMatrixToInverse[11] * aMatrixToInverse[14] +
			aMatrixToInverse[9] * aMatrixToInverse[2] * aMatrixToInverse[15] -
			aMatrixToInverse[9] * aMatrixToInverse[3] * aMatrixToInverse[14] -
			aMatrixToInverse[13] * aMatrixToInverse[2] * aMatrixToInverse[11] +
			aMatrixToInverse[13] * aMatrixToInverse[3] * aMatrixToInverse[10];

		inv[5] = aMatrixToInverse[0] * aMatrixToInverse[10] * aMatrixToInverse[15] -
			aMatrixToInverse[0] * aMatrixToInverse[11] * aMatrixToInverse[14] -
			aMatrixToInverse[8] * aMatrixToInverse[2] * aMatrixToInverse[15] +
			aMatrixToInverse[8] * aMatrixToInverse[3] * aMatrixToInverse[14] +
			aMatrixToInverse[12] * aMatrixToInverse[2] * aMatrixToInverse[11] -
			aMatrixToInverse[12] * aMatrixToInverse[3] * aMatrixToInverse[10];

		inv[9] = -aMatrixToInverse[0] * aMatrixToInverse[9] * aMatrixToInverse[15] +
			aMatrixToInverse[0] * aMatrixToInverse[11] * aMatrixToInverse[13] +
			aMatrixToInverse[8] * aMatrixToInverse[1] * aMatrixToInverse[15] -
			aMatrixToInverse[8] * aMatrixToInverse[3] * aMatrixToInverse[13] -
			aMatrixToInverse[12] * aMatrixToInverse[1] * aMatrixToInverse[11] +
			aMatrixToInverse[12] * aMatrixToInverse[3] * aMatrixToInverse[9];

		inv[13] = aMatrixToInverse[0] * aMatrixToInverse[9] * aMatrixToInverse[14] -
			aMatrixToInverse[0] * aMatrixToInverse[10] * aMatrixToInverse[13] -
			aMatrixToInverse[8] * aMatrixToInverse[1] * aMatrixToInverse[14] +
			aMatrixToInverse[8] * aMatrixToInverse[2] * aMatrixToInverse[13] +
			aMatrixToInverse[12] * aMatrixToInverse[1] * aMatrixToInverse[10] -
			aMatrixToInverse[12] * aMatrixToInverse[2] * aMatrixToInverse[9];

		inv[2] = aMatrixToInverse[1] * aMatrixToInverse[6] * aMatrixToInverse[15] -
			aMatrixToInverse[1] * aMatrixToInverse[7] * aMatrixToInverse[14] -
			aMatrixToInverse[5] * aMatrixToInverse[2] * aMatrixToInverse[15] +
			aMatrixToInverse[5] * aMatrixToInverse[3] * aMatrixToInverse[14] +
			aMatrixToInverse[13] * aMatrixToInverse[2] * aMatrixToInverse[7] -
			aMatrixToInverse[13] * aMatrixToInverse[3] * aMatrixToInverse[6];

		inv[6] = -aMatrixToInverse[0] * aMatrixToInverse[6] * aMatrixToInverse[15] +
			aMatrixToInverse[0] * aMatrixToInverse[7] * aMatrixToInverse[14] +
			aMatrixToInverse[4] * aMatrixToInverse[2] * aMatrixToInverse[15] -
			aMatrixToInverse[4] * aMatrixToInverse[3] * aMatrixToInverse[14] -
			aMatrixToInverse[12] * aMatrixToInverse[2] * aMatrixToInverse[7] +
			aMatrixToInverse[12] * aMatrixToInverse[3] * aMatrixToInverse[6];

		inv[10] = aMatrixToInverse[0] * aMatrixToInverse[5] * aMatrixToInverse[15] -
			aMatrixToInverse[0] * aMatrixToInverse[7] * aMatrixToInverse[13] -
			aMatrixToInverse[4] * aMatrixToInverse[1] * aMatrixToInverse[15] +
			aMatrixToInverse[4] * aMatrixToInverse[3] * aMatrixToInverse[13] +
			aMatrixToInverse[12] * aMatrixToInverse[1] * aMatrixToInverse[7] -
			aMatrixToInverse[12] * aMatrixToInverse[3] * aMatrixToInverse[5];

		inv[14] = -aMatrixToInverse[0] * aMatrixToInverse[5] * aMatrixToInverse[14] +
			aMatrixToInverse[0] * aMatrixToInverse[6] * aMatrixToInverse[13] +
			aMatrixToInverse[4] * aMatrixToInverse[1] * aMatrixToInverse[14] -
			aMatrixToInverse[4] * aMatrixToInverse[2] * aMatrixToInverse[13] -
			aMatrixToInverse[12] * aMatrixToInverse[1] * aMatrixToInverse[6] +
			aMatrixToInverse[12] * aMatrixToInverse[2] * aMatrixToInverse[5];

		inv[3] = -aMatrixToInverse[1] * aMatrixToInverse[6] * aMatrixToInverse[11] +
			aMatrixToInverse[1] * aMatrixToInverse[7] * aMatrixToInverse[10] +
			aMatrixToInverse[5] * aMatrixToInverse[2] * aMatrixToInverse[11] -
			aMatrixToInverse[5] * aMatrixToInverse[3] * aMatrixToInverse[10] -
			aMatrixToInverse[9] * aMatrixToInverse[2] * aMatrixToInverse[7] +
			aMatrixToInverse[9] * aMatrixToInverse[3] * aMatrixToInverse[6];

		inv[7] = aMatrixToInverse[0] * aMatrixToInverse[6] * aMatrixToInverse[11] -
			aMatrixToInverse[0] * aMatrixToInverse[7] * aMatrixToInverse[10] -
			aMatrixToInverse[4] * aMatrixToInverse[2] * aMatrixToInverse[11] +
			aMatrixToInverse[4] * aMatrixToInverse[3] * aMatrixToInverse[10] +
			aMatrixToInverse[8] * aMatrixToInverse[2] * aMatrixToInverse[7] -
			aMatrixToInverse[8] * aMatrixToInverse[3] * aMatrixToInverse[6];

		inv[11] = -aMatrixToInverse[0] * aMatrixToInverse[5] * aMatrixToInverse[11] +
			aMatrixToInverse[0] * aMatrixToInverse[7] * aMatrixToInverse[9] +
			aMatrixToInverse[4] * aMatrixToInverse[1] * aMatrixToInverse[11] -
			aMatrixToInverse[4] * aMatrixToInverse[3] * aMatrixToInverse[9] -
			aMatrixToInverse[8] * aMatrixToInverse[1] * aMatrixToInverse[7] +
			aMatrixToInverse[8] * aMatrixToInverse[3] * aMatrixToInverse[5];

		inv[15] = aMatrixToInverse[0] * aMatrixToInverse[5] * aMatrixToInverse[10] -
			aMatrixToInverse[0] * aMatrixToInverse[6] * aMatrixToInverse[9] -
			aMatrixToInverse[4] * aMatrixToInverse[1] * aMatrixToInverse[10] +
			aMatrixToInverse[4] * aMatrixToInverse[2] * aMatrixToInverse[9] +
			aMatrixToInverse[8] * aMatrixToInverse[1] * aMatrixToInverse[6] -
			aMatrixToInverse[8] * aMatrixToInverse[2] * aMatrixToInverse[5];

		T det = aMatrixToInverse[0] * inv[0] + aMatrixToInverse[1] * inv[4] + aMatrixToInverse[2] * inv[8] + aMatrixToInverse[3] * inv[12];

		det = 1.0f / det;

		Matrix44<T> returnMatrix;

		for (short i = 0; i < 16; i++)
		{
			returnMatrix[i] = inv[i] * det;
		}

		return returnMatrix;
	}
}
