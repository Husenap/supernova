#pragma once
#include "Vector.h"

namespace CommonUtilities
{
	class CBounds
	{
	public:
		CBounds();
		~CBounds();

		void SetMinMax(const Vector3f& aMin, const Vector3f& aMax);
		void Encapsulate(const Vector3f& aPosition);
		void Inflate(float aInflation);
		void EncapsulateWithoutRecalculating(const Vector3f& aPosition);

		void RecalculateCenterAndExtents();

		bool Contains(const Vector3f& aPosition) const;
		Vector3f GetClosestPoint(const Vector3f& aPoint) const;

		const Vector3f& GetCenter() const { return myCenter; }
		const Vector3f& GetExtents() const { return myExtents; }
		const Vector3f& GetMin() const { return myMin; }
		const Vector3f& GetMax() const { return myMax; }

	private:
		Vector3f myCenter;
		Vector3f myExtents;
		Vector3f myMin;
		Vector3f myMax;
	};
}