#include "Mathf.h"
#include <math.h>

namespace CommonUtilities
{
	CommonUtilities::CFrustum BuildFrustumFromViewProjectionMatrix(const CommonUtilities::Matrix44f & aViewProjectionMatrix)
	{
		CommonUtilities::CFrustum frustum;
		frustum.Init(aViewProjectionMatrix);
		return frustum;
	}

	CommonUtilities::Matrix44f CreatePerspectiveProjectionMatrix(float aFov, float aAspect, float aNear, float aFar)
	{
		float B = 1.f / std::tan(aFov * CommonUtilities::Deg2Rad * 0.5f);
		float A = B / aAspect;
		float C = aFar / (aFar - aNear);
		float D = 1.f;
		float E = -aNear * aFar / (aFar - aNear);

		CommonUtilities::Matrix44<float> projectionMatrix(
			A, 0, 0, 0,
			0, B, 0, 0,
			0, 0, C, D,
			0, 0, E, 0);

		return projectionMatrix;
	}

	CommonUtilities::Matrix44f CreateOrthographicProjectionMatrix(float aSize, float aAspect, float aNear, float aFar)
	{
		float B = 2.f / aSize;
		float A = 2 / (aSize*aAspect);
		float C = 1.f / (aFar - aNear);
		float E = aNear / (aNear - aFar);

		CommonUtilities::Matrix44<float> projectionMatrix(
			A, 0, 0, 0,
			0, B, 0, 0,
			0, 0, C, 0,
			0, 0, E, 1);

		return projectionMatrix;
	}

	const CommonUtilities::Matrix44f LookAt(const CommonUtilities::Vector3f & aPositionToLookAt, const CommonUtilities::Vector3f aPositionToLookFrom, const CommonUtilities::Vector3f & aUpVector)
	{
		CommonUtilities::Vector3f  forward = (aPositionToLookAt - aPositionToLookFrom).GetNormalized();
		CommonUtilities::Vector3f  up = aUpVector.GetNormalized();
		CommonUtilities::Vector3f  right = up.Cross(forward).GetNormalized();
		up = forward.Cross(right);

		CommonUtilities::Matrix44f Result;
		Result[0] = right.x;
		Result[1] = right.y;
		Result[2] = right.z;
		Result[4] = up.x;
		Result[5] = up.y;
		Result[6] = up.z;
		Result[8] = forward.x;
		Result[9] = forward.y;
		Result[10] = forward.z;
		Result[12] = aPositionToLookFrom.x;
		Result[13] = aPositionToLookFrom.y;
		Result[14] = aPositionToLookFrom.z;
		return Result;
	}
}