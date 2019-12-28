#pragma once

#include "Vector.h"

namespace CommonUtilities
{
	template <typename T>
	class Plane
	{
	public:
		Plane();
		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);
		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aNormal);
		
		void InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);
		void InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal);
		void InitWithABCD(float aA, float aB, float aC, float aD);

		bool Inside(const Vector3<T>& aPosition) const;
		bool Outside(const Vector3<T>& aPosition) const;
		bool OnPlane(const Vector3<T>& aPosition) const;
		T Distance(const Vector3<T>& aPosition) const;

		const CommonUtilities::Vector3f& GetPoint() const { return myPoint; }
		const CommonUtilities::Vector3f& GetNormal() const { return myNormal; }

	private:
		Vector3<T> myPoint;
		Vector3<T> myNormal;

	};

	template<typename T>
	inline Plane<T>::Plane()
	{
		myPoint = Vector3<T>();
		myNormal = Vector3<T>(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));
	}

	template<typename T>
	inline Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		myPoint = aPoint0;
		myNormal = (aPoint1 - aPoint0).Cross(aPoint2 - aPoint0);
	}

	template<typename T>
	inline Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aNormal)
	{
		myPoint = aPoint0;
		myNormal = aNormal;
	}

	template<typename T>
	inline void Plane<T>::InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		myPoint = aPoint0;
		myNormal = (aPoint1 - aPoint0).Cross(aPoint2 - aPoint0);
	}

	template<typename T>
	inline void Plane<T>::InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
	{
		myPoint = aPoint;
		myNormal = aNormal;
	}

	template<typename T>
	inline void Plane<T>::InitWithABCD(float aA, float aB, float aC, float aD)
	{
		myNormal = { aA, aB, aC };
		aD /= myNormal.Length();
		myNormal.Normalize();
		myPoint = myNormal * aD;
	}

	/*
	* p*n - q*n = 0;
	* ax + by + cz - d = 0
	*/
	template<typename T>
	inline bool Plane<T>::Inside(const Vector3<T>& aPosition) const
	{
		return aPosition.Dot(myNormal) - myPoint.Dot(myNormal) < static_cast<T>(0);
	}

	template<typename T>
	inline bool Plane<T>::Outside(const Vector3<T>& aPosition) const
	{
		return aPosition.Dot(myNormal) - myPoint.Dot(myNormal) > static_cast<T>(0);
	}

	template<typename T>
	inline bool Plane<T>::OnPlane(const Vector3<T>& aPosition) const
	{
		return aPosition.Dot(myNormal) - myPoint.Dot(myNormal) == static_cast<T>(0);
	}

	template<typename T>
	inline T Plane<T>::Distance(const Vector3<T>& aPosition) const
	{
		return aPosition.Dot(myNormal) - myPoint.Dot(myNormal);
	}
}
