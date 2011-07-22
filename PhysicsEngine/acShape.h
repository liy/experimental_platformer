#pragma once
#include "acMath.h"

/*
* Copyright (c) 2006-2009 Erin Catto http://www.gphysics.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

// forward declaration
template<typename T>struct acAABB2;
typedef struct acAABB2<float> AABB2f;

struct RayCastInput{
	Vec2f p1, p2;
	float maxFraction;
};

struct RayCastOutput{
	Vec2f normal;
	float fraction;
};

/// Modified from Box2D
template<typename T>
struct acAABB2{
	// centre of the AABB
	Vec2<T> GetCentre() const{
		return (lowerBound + upperBound) * 0.5f;
	}

	// half extent of the AABB
	Vec2<T> GetExtents() const{
		return (upperBound - lowerBound) * 0.5f;
	}

	// Combine two AABB into one.
	void Combine(const acAABB2<T>& aabb1, const acAABB2<T>& aabb2){
		lowerBound = acMin(aabb1.lowerBound, aabb2.lowerBound);
		upperBound = acMax(aabb1.upperBound, aabb2.upperBound);
	}

	// Whether the AABB is contained in this
	bool Contains(const acAABB2<T>& aabb) const{
		return lowerBound.x <= aabb.lowerBound.x && lowerBound.y <= aabb.lowerBound.y &&
			upperBound.x >= aabb.upperBound.x && upperBound.y >= aabb.upperBound.y;
	}

	// Contains the point vector?
	// countForTouch is true, then the point on the edge will still be recognized as contained inside.
	bool Contains(const Vec2<T> v, bool countForTouch) const{
		if(countForTouch){
			return lowerBound.x <= v.x && lowerBound.y <= v.y && upperBound.x >= v.x && upperBound.y >= v.y;
		}
		else{
			return lowerBound.x < v.x && lowerBound.y < v.y && upperBound.x > v.x && upperBound.y > v.y;
		}
	}

	// copy and modified from Box2D
	bool RayCast(RayCastOutput* output, const RayCastInput& input){
		float tmin = -FLT_MAX;
		float tmax = FLT_MAX;
   
		Vec2f p = input.p1;
		Vec2f d = input.p2 - input.p1;
		Vec2f absD = acAbs(d);

		Vec2f normal;

		for (int i = 0; i < 2; ++i)
		{
			if (absD[i] < FLT_EPSILON)
			{
				// Parallel.
				if (p[i] < lowerBound[i] || upperBound[i] < p[i])
				{
					return false;
				}
			}
			else
			{
				float inv_d = 1.0f / d[i];
				float t1 = (lowerBound[i] - p[i]) * inv_d;
				float t2 = (upperBound[i] - p[i]) * inv_d;

				// Sign of the normal vector.
				float s = -1.0f;

				if (t1 > t2)
				{
					swap(t1, t2);
					s = 1.0f;
				}

				// Push the min up
				if (t1 > tmin)
				{
					normal.SetZero();
					normal[i] = s;
					tmin = t1;
				}

				// Pull the max down
				tmax = acMin(tmax, t2);

				if (tmin > tmax)
				{
					return false;
				}
			}
		}

		// Does the ray start inside the box?
		// Does the ray intersect beyond the max fraction?
		if (tmin < 0.0f || input.maxFraction < tmin)
		{
			return false;
		}

		// Intersection.
		output->fraction = tmin;
		output->normal = normal;
		return true;
	}

	Vec2<T> lowerBound;
	Vec2<T> upperBound;
};

// Two AABB's Minkowski Difference calculation, create a new AABB.
// the result AABB is the a - b.
// If you are casting a ray to from on AABB to another AABB to check for TOI, then
// the from AABB should be b, to AABB should be a
inline AABB2f operator- (const AABB2f& a, const AABB2f& b){
	AABB2f newAABB;
	newAABB.lowerBound = a.lowerBound - b.upperBound;
	newAABB.upperBound = a.upperBound - b.lowerBound;
	return newAABB;
}

class acShape
{
public:

	enum Type{
		type_unknown = -1,
		type_circle = 0,
		type_polygon = 1,
	};

	acShape(void){type = type_unknown;};

	virtual ~acShape(void){};

	Type type;

	// =0 means abstract method which needs implementation.
	// acBody will use this method to compute AABB in the update process.
	virtual void ComputeAABB(AABB2f* aabb, const Transform2f& tf) const =0;

	// virtual deep copy clone method.
	virtual acShape* Clone() const =0;
};

