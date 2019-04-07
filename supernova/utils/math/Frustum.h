#pragma once

#include <vector>
#include "Matrix.h"
#include "Plane.h"
#include "Bounds.h"

namespace CommonUtilities
{

	class CFrustum
	{
	public:
		CFrustum();
		~CFrustum();

		void Init(const CommonUtilities::Matrix44f& aViewProjectionMatrix);

		bool CullSphere(const CommonUtilities::Vector3f& aPosition, float aRadius) const;
		bool CullAABB(const CommonUtilities::Vector3f& aMin, const CommonUtilities::Vector3f& aMax) const;

	private:
		CommonUtilities::Plane<float> myFrustum[6];


	};

}
