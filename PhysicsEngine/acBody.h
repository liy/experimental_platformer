#pragma once

#include "acShape.h"

class acBody
{
public:
	acBody(void);
	~acBody(void);

	AABB2f aabb;

	void* userData;

	float friction;

	float restitution;

	float rotation;

	Vec2f position;

	Vec2f velocity;

	float damping;

	// clone the shape and assign it to the body
	void SetShape(const acShape* shape);

	// Draw the AABB bounding box
	void DrawAABB(float r, float g, float b);

	// keep AABB, transform synchronize
	void Synchronize();

protected:
	// The shape will be cloned. SetShape() will make the shape clone.
	const acShape* shape_ptr;

};

