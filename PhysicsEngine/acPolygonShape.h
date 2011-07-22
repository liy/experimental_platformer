#pragma once
#include "acShape.h"

// Copy constructor, and copy assignment constructor should be correctly auto generated, since there is no pointer or anything special.
class acPolygonShape : public acShape
{
public:
	acPolygonShape(void);

	~acPolygonShape(void);

	void SetVertices(const Vec2f* vertices, int count);

	void SetAsBox(float hw, float hh);

	// compute AABB. The result AABB vertices are transformed into world coordinate
	virtual void ComputeAABB(AABB2f* aabb, const Transform2f& tf) const;

	// find the furtheset vertex along search direction vector sd. The vertex is transformed into world coordinate
	const Vec2f& GetSupport(const Vec2f& sd) const;

	// clone the polygon, used internally when it is assigned to body.
	acPolygonShape* Clone() const;

	Vec2f vertices[8];
	Vec2f normals[8];
	int numVertices;
};

