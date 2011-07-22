#pragma once
#include "acBody.h"

/// First we turn two AABBs into one single Minkowski Difference AABB.
// If two body penetrate each other, the MD AABB will contain the origin(0, 0). Touch collision will be ignored
// If contains the origin. We only need to find the smallest absolute x and y components of upperBound and lowerBound.
// It will give ust the minimum translation and the normal.
bool AABBSAT(const AABB2f& aabb1, const AABB2f& aabb2, Vec2f& normal, Vec2f& minTransV){
	// create a minkowski difference AABB
	AABB2f aabb = aabb1 - aabb2;

	float minD = FLT_MAX;
	if(aabb.Contains(vec2_zero, false)){
		
		Vec2f absLowerBound = acAbs(aabb.lowerBound);
		Vec2f absUpperBound = acAbs(aabb.upperBound);

		for(int i=0; i<2; ++i){
			float s = 1.0f;
			if(absLowerBound[i] < absUpperBound[i]){
				if(absLowerBound[i] < minD){
					s = -1.0f;

					minD = absLowerBound[i];
					normal.SetZero();
					normal[i] = s;
				}
			}
			else{
				if(absUpperBound[i] < minD){
					minD = absUpperBound[i];
					normal.SetZero();
					normal[i] = s;
				}
			}
		}

		minTransV = normal * minD;
		return true;
	}
	return false;
}

// Another version of SAT.
bool SAT(const acBody& a, const acBody& b, Vec2f& normal, float& sd){
	Vec2f aExtent = a.aabb.GetExtents();
	Vec2f bExtent = b.aabb.GetExtents();

	Vec2f dv = b.position - a.position;
	Vec2f absDv = acAbs(dv);

	float xd = (aExtent.x + bExtent.x) - absDv.x;
	float yd = (aExtent.y + bExtent.y) - absDv.y;

	if(xd > 0 && yd > 0){
		normal.SetZero();

		Vec2f nx(1.0f, 0.0f);
		Vec2f ny(0.0f, 1.0f);
		// minimum sepereate vector is along yaxis
		if(yd < xd){
			if(dv.y < 0){
				normal.y = 1.0f;
			}
			else{
				normal.y = -1.0f;
			}
			sd = yd;
		}
		// minimum sepereate vector is along x axis
		else{
			if(dv.x < 0){
				normal.x = 1.0f;
			}
			else{
				normal.x = -1.0f;
			}
			sd = xd;
		}
		return true;
	}
	else{
		return false;
	}
}