#pragma once

#include "Vector.h"
#include "Matrix.h"

#include "Frustum.h"

namespace CommonUtilities
{
	template<typename T>
	T Clamp(T aNumber, T aMin, T aMax)
	{
		if( aNumber < aMin )
			return aMin;
		if( aNumber > aMax )
			return aMax;
		return aNumber;
	}
	template<typename T>
	T Clamp01(T aNumber)
	{
		return Clamp(aNumber, T(0), T(1));
	}

	template<typename T>
	const T Lerp(T aStart, T aEnd, float aTime)
	{
		return aStart + (aEnd-aStart)*aTime;
	}
	// aStart and aEnd - These need to be normalized!
	template<typename T>
	const T Slerp(T aStart, T aEnd, float aTime)
	{
		// Dot product - the cosine of the angle between 2 vectors.
		float dot = aStart.Dot(aEnd);
		// Clamp it to be in the range of Acos()
		// This may be unnecessary, but floating point
		// precision can be a fickle mistress.
		dot = Clamp(dot, -1.0f, 1.0f);
		// Acos(dot) returns the angle between start and end,
		// And multiplying that by percent returns the angle between
		// start and the final result.
		float theta = std::acos(dot)*aTime;
		CommonUtilities::Vector3f RelativeVec = aEnd - aStart*dot;
		RelativeVec.Normalize();     // Orthonormal basis
		return ((aStart*std::cos(theta)) + (RelativeVec*sin(theta)));
	}

	template<typename T>
	const T Min(T a, T b)
	{
		return a < b ? a : b;
	}
	template<typename T>
	const T Max(T a, T b)
	{
		return a > b ? a : b;
	}

	CommonUtilities::CFrustum BuildFrustumFromViewProjectionMatrix(const CommonUtilities::Matrix44f& aViewProjectionMatrix);

	CommonUtilities::Matrix44f CreatePerspectiveProjectionMatrix(float aFov, float aAspect, float aNear, float aFar);
	CommonUtilities::Matrix44f CreateOrthographicProjectionMatrix(float aSize, float aAspect, float aNear, float aFar);

	const CommonUtilities::Matrix44f LookAt(const CommonUtilities::Vector3f & aPositionToLookAt, const CommonUtilities::Vector3f aPositionToLookFrom, const CommonUtilities::Vector3f & aUpVector = CommonUtilities::Vector3f(0.0f, 1.0f, 0.0f));
	const float Pif = 3.14159265358979323846f;
	const float Deg2Rad = Pif / 180.f;
	const float Rad2Deg = 180.f / Pif;
}
