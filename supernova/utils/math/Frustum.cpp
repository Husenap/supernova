#include "Frustum.h"


namespace CommonUtilities
{
	CFrustum::CFrustum()
	{
	}

	CFrustum::~CFrustum()
	{
	}

	void CFrustum::Init(const CommonUtilities::Matrix44f & aViewProjectionMatrix)
	{
		const auto& viewProjMat = aViewProjectionMatrix;

		CommonUtilities::Vector4f planes[6];

		// Left clipping plane
		planes[0].x = viewProjMat[3] + viewProjMat[0];
		planes[0].y = viewProjMat[7] + viewProjMat[4];
		planes[0].z = viewProjMat[11] + viewProjMat[8];
		planes[0].w = viewProjMat[15] + viewProjMat[12];
		// Right clipping plane
		planes[1].x = viewProjMat[3] - viewProjMat[0];
		planes[1].y = viewProjMat[7] - viewProjMat[4];
		planes[1].z = viewProjMat[11] - viewProjMat[8];
		planes[1].w = viewProjMat[15] - viewProjMat[12];
		// Top clipping plane
		planes[2].x = viewProjMat[3] - viewProjMat[1];
		planes[2].y = viewProjMat[7] - viewProjMat[5];
		planes[2].z = viewProjMat[11] - viewProjMat[9];
		planes[2].w = viewProjMat[15] - viewProjMat[13];
		// Bottom clipping plane
		planes[3].x = viewProjMat[3] + viewProjMat[1];
		planes[3].y = viewProjMat[7] + viewProjMat[5];
		planes[3].z = viewProjMat[11] + viewProjMat[9];
		planes[3].w = viewProjMat[15] + viewProjMat[13];
		// Near clipping plane
		planes[4].x = viewProjMat[3];
		planes[4].y = viewProjMat[7];
		planes[4].z = viewProjMat[11];
		planes[4].w = viewProjMat[15];
		// Far clipping plane
		planes[5].x = viewProjMat[3] - viewProjMat[2];
		planes[5].y = viewProjMat[7] - viewProjMat[6];
		planes[5].z = viewProjMat[11] - viewProjMat[10];
		planes[5].w = viewProjMat[15] - viewProjMat[14];


		for (int i = 0; i < 6; ++i)
		{
			myFrustum[i].InitWithABCD(-planes[i].x, -planes[i].y, -planes[i].z, planes[i].w);
		}
	}

	bool CFrustum::CullSphere(const CommonUtilities::Vector3f & aPosition, float aRadius) const
	{
		for (unsigned short i = 0; i < 6; ++i)
		{
			const auto& plane = myFrustum[i];
			if (plane.Distance(aPosition) > aRadius)
			{
				return true;
			}
		}

		return false;
	}

	bool CFrustum::CullAABB(const CommonUtilities::Vector3f & aMin, const CommonUtilities::Vector3f & aMax) const
	{
		const CommonUtilities::Vector3f* box[2] = { &aMin, &aMax };

		for (int i = 0; i < 6; i++)
		{
			const auto& plane = myFrustum[i];

			int out = 0;
			out += (plane.Outside({aMin.x, aMin.y, aMin.z}) ? 1 : 0);
			out += (plane.Outside({aMax.x, aMin.y, aMin.z}) ? 1 : 0);
			out += (plane.Outside({aMin.x, aMax.y, aMin.z}) ? 1 : 0);
			out += (plane.Outside({aMax.x, aMax.y, aMin.z}) ? 1 : 0);
			out += (plane.Outside({aMin.x, aMin.y, aMax.z}) ? 1 : 0);
			out += (plane.Outside({aMax.x, aMin.y, aMax.z}) ? 1 : 0);
			out += (plane.Outside({aMin.x, aMax.y, aMax.z}) ? 1 : 0);
			out += (plane.Outside({aMax.x, aMax.y, aMax.z}) ? 1 : 0);
			if (out == 8) return true;
		}

		return false;
	}
}
