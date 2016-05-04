#pragma once
#include <math.h>
#include "mymath\mymath.h"

class Triangle
{
public:
	static float CalcArea(float edgeALength, float edgeBLength, float edgeCLength)
	{
		float s = (edgeALength + edgeBLength + edgeCLength) * 0.5f;
		return sqrtf(s * (s - edgeALength) * (s - edgeBLength) * (s - edgeCLength));
	}

	static float CalcArea(const mm::vec3& pointA, const mm::vec3& pointB, const mm::vec3& pointC)
	{
		float edgeALength = mm::length(pointA - pointB);
		float edgeBLength = mm::length(pointB - pointC);
		float edgeCLength = mm::length(pointC - pointA);
		return CalcArea(edgeALength, edgeBLength, edgeCLength);
	}

	static mm::vec3 CalcNormal(const mm::vec3& pointA, const mm::vec3& pointB, const mm::vec3& pointC, bool bClockwise = true)
	{
		mm::vec3 edgeA = pointB - pointA;
		mm::vec3 edgeB = pointC - pointA;

		if (bClockwise)
			return mm::normalize(mm::cross(edgeB, edgeA));
		else
			return mm::normalize(mm::cross(edgeA, edgeB));
	}

	static mm::vec3 CalcTangent(const mm::vec3& pointA, const mm::vec3& pointB, const mm::vec3& pointC, const mm::vec2& uvA, const mm::vec2& uvB, const mm::vec2& uvC)
	{
		//world space vektoraink
		mm::vec3 e0 = mm::vec3(pointB.x - pointA.x, pointB.y - pointA.y, pointB.z - pointA.z);
		mm::vec3 e1 = mm::vec3(pointC.x - pointA.x, pointC.y - pointA.y, pointC.z - pointA.z);
		
		//delta textura koordiánátáink
		float dU0 = uvB.x - uvA.x;
		float dV0 = uvB.y - uvA.y;
		
		float dU1 = uvC.x - uvA.x;
		float dV1 = uvC.y - uvA.y;
		
		float fDiv = (dU0 * dV1 - dU1 * dV0);
		
		mm::vec3 tangent(0,0,0);
		if (fDiv != 0.f)
		{
			// Calculate the denominator of the tangent/binormal equation.
			float den = 1.0f / fDiv;
		
			// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
			tangent.x = (dV1 * e0.x - dV0 * e1.x) * den;
			tangent.y = (dV1 * e0.y - dV0 * e1.y) * den;
			tangent.z = (dV1 * e0.z - dV0 * e1.z) * den;
		
			float fLen = sqrt(tangent.x * tangent.x + tangent.y * tangent.y + tangent.z * tangent.z);
		
			if (fLen > 0)
			{
				tangent /= fLen;
			}
		}

		return tangent;
	}
};
