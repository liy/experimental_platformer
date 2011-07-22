#include "acPolygonShape.h"
#include <cassert>

acPolygonShape::acPolygonShape(void)
{
	/*
	type = type_polygon;
	numVertices = 0;
	*/
	SetAsBox(16.0f, 16.0f);
}


acPolygonShape::~acPolygonShape(void)
{

}

void acPolygonShape::SetVertices(const Vec2f* vs, int count){
	numVertices = count;
	
	for(int i=0; i<numVertices; ++i){
		vertices[i] = vs[i];
	}

	for(int i=0; i<numVertices; ++i){
		// next vertex index.
		int nextIndex = i+1;
		// Since polygon is a loop, the last vertex index will be connect to the first index.
		if(nextIndex >= numVertices){
			nextIndex = 0;
		}

		// ccw edge, direction pointed to the next vertex
		Vec2f edge = vertices[nextIndex] - vertices[i];
		// if the edge's length is close enough to 0. Then report an error.... Input vertices error.
		assert(edge.LengthSquared() > FLT_EPSILON * FLT_EPSILON);

		// since this is CCW polygon, the normal should lay on the right of the edge
		normals[i] = Cross(edge, 1.0f);
		// normalize the normal.
		normals[i].Normalize();
	}
}

void acPolygonShape::SetAsBox(float hw, float hh){
	numVertices = 4;
	// ccw vertices arrangement
	vertices[0].Set(-hw, -hh);//  bottom left
	vertices[1].Set(hw, -hh);// bottom right
	vertices[2].Set(hw, hh);// top right
	vertices[3].Set(-hw, hh);// top left
	// normals
	normals[0].Set(0.0f, -1.0f);// bottom edge
	normals[1].Set(1.0f, 0.0f);// right edge
	normals[2].Set(0.0f, 1.0f);// top edge
	normals[3].Set(-1.0f, 0.0f);// left edge
}

void acPolygonShape::ComputeAABB(AABB2f* aabb, const Transform2f& tf) const{
	// initilize the lower and upper bounds to the same corner vector.
	Vec2f lowerBound = tf.Multiply(vertices[0]);
	Vec2f upperBound = lowerBound;

	for(int i=0; i<numVertices; ++i){
		Vec2f v = tf.Multiply(vertices[i]);
		// Keep looking for the uppoer and lower bounds
		lowerBound = acMin(lowerBound, v);
		upperBound = acMax(upperBound, v);
	}
	// set the AABB
	aabb->lowerBound = lowerBound;
	aabb->upperBound = upperBound;
}

const Vec2f& acPolygonShape::GetSupport(const Vec2f& sd) const{
	int furthestIndex = 0;
	float furthestValue = vertices[0].Dot(sd);
	for(int i=0; i<numVertices; ++i){
		float value = vertices[i].Dot(sd);
		if(value > furthestValue){
			furthestIndex = i;
			furthestValue = value;
		}
	}
	return vertices[furthestIndex];
}

acPolygonShape* acPolygonShape::Clone() const{
	// The program auto generated copy constructor should work.
	return new acPolygonShape(*this);
}