#pragma once

#include <Vector2.hpp>

namespace MathHelper
{

	template <typename T>
	static constexpr T locPi = static_cast<T>(3.14159265358979323846264);

	static constexpr float locPif = locPi<float>;
	static constexpr double locPid = locPi<double>;

	template <typename T>
	static constexpr T RadToDeg(T someRad)
	{
		return someRad / locPi<T> * static_cast<T>(180);
	}

	template <typename T>
	static constexpr T DegToRad(T someDeg)
	{
		return someDeg / static_cast<T>(180) * locPi<T>;
	}

	template <typename T>
	static constexpr T Lerp(T aStartValue, T anEndValue, float aPercentage)
	{
		return aStartValue + (anEndValue - aStartValue) * aPercentage;
	}

	template <typename T>
	static constexpr T Clamp(T aValue, T someMin, T someMax)
	{
		return aValue < someMin ? someMin : aValue > someMax ? someMax : aValue;
	}

	template <typename T>
	int Signum(T val) {
		return (T(0) < val) - (val < T(0));
	}

	static CU::Vector2<float> MoveTowards(CU::Vector2<float> aValue, CU::Vector2<float> aTarget, float aMaxDelta)
	{
		CU::Vector2<float> toTarget = aTarget - aValue;
		float length = toTarget.Length();

		if (length <= 0.0f)
		{
			return aTarget;
		}

		toTarget /= length;

		if (length > aMaxDelta)
		{
			length = aMaxDelta;
		}

		return aValue + toTarget * length;
	}
}