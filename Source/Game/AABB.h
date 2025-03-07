#pragma once

#include <Vector2.hpp>

class AABB
{
public:

	static AABB FromCenterAndSize(const CU::Vector2<float>& aCenter, const CU::Vector2<float>& aSize)
	{
		AABB aabb;

		aabb.myMin = aCenter - aSize * 0.5f;
		aabb.myMax = aabb.myMin + aSize;

		return aabb;
	}

	AABB() = default;
	AABB(const CU::Vector2<float>& aMin, const CU::Vector2<float>& aMax)
		: myMin(aMin),
		myMax(aMax)
	{}

	bool Contains(const CU::Vector2<float>& aPoint)
	{
		return
			myMax.x >= aPoint.x && myMin.x <= aPoint.x &&
			myMax.y >= aPoint.y && myMin.y <= aPoint.y;
	}

	bool Intersects(const AABB& anOther)
	{
		return
			myMax.x >= anOther.myMin.x && myMin.x <= anOther.myMax.x
			&& myMax.y >= anOther.myMin.y && myMin.y <= anOther.myMax.y;
	}

	void Extend(const AABB& anOther)
	{
		myMin.x = min(myMin.x, anOther.myMin.x);
		myMin.y = min(myMin.y, anOther.myMin.y);

		myMax.x = max(myMax.x, anOther.myMax.x);
		myMax.y = max(myMax.y, anOther.myMax.y);
	}

	void Expand(const CU::Vector2<float>& anExpansion)
	{
		myMin.x -= anExpansion.x * 0.5f;
		myMax.x += anExpansion.x * 0.5f;

		myMin.y -= anExpansion.y * 0.5f;
		myMax.y += anExpansion.y * 0.5f;
	}

	void Shift(const CU::Vector2<float>& aDisplacement)
	{
		myMin += aDisplacement;
		myMax += aDisplacement;
	}

	void ClipInside(const AABB& aClipBox)
	{
		float tx = 0.0f;
		float ty = 0.0f;

		tx = max(aClipBox.GetMin().x - GetMin().x, 0.0f);
		ty = max(aClipBox.GetMin().y - GetMin().y, 0.0f);

		myMin = { min(myMin.x + tx, myMax.x), min(myMin.y + ty, myMax.y) };

		tx = min(aClipBox.GetMax().x - GetMax().x, 0.0f);
		ty = min(aClipBox.GetMax().y - GetMax().y, 0.0f);

		myMax = { max(myMax.x + tx, myMin.x), max(myMax.y + ty, myMin.y) };
	}

	inline CU::Vector2<float> GetSize() const { return myMax - myMin; }
	inline CU::Vector2<float> GetCenter() const { return myMin + GetSize() * 0.5f; }

	inline const CU::Vector2<float>& GetMin() const { return myMin; }
	inline const CU::Vector2<float>& GetMax() const { return myMax; }

private:

	CU::Vector2<float> myMin;
	CU::Vector2<float> myMax;

};
