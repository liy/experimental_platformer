#include "acBody.h"
#include "acPolygonShape.h"
#include <GL\glut.h>
#include <GL\GL.h>

acBody::acBody(void): friction(0.2f), restitution(0.1f), damping(0.0f), rotation(0.0f)
{
	position.SetZero();
	velocity.SetZero();
}


acBody::~acBody(void)
{
	if(shape_ptr != NULL)
		delete shape_ptr;
}

// clone the shape and assigne it to the body
void acBody::SetShape(const acShape* shape){
	shape_ptr = shape->Clone();
	// update the AABB
	Synchronize();
}

void acBody::DrawAABB(float r, float g, float b){
	glPushMatrix();
	glLoadIdentity();

	glColor3f(r, g, b);

	glBegin(GL_LINE_LOOP);
	glVertex2f(aabb.lowerBound.x, aabb.lowerBound.y);
	glVertex2f(aabb.lowerBound.x, aabb.upperBound.y);
	glVertex2f(aabb.upperBound.x, aabb.upperBound.y);
	glVertex2f(aabb.upperBound.x, aabb.lowerBound.y);
	glEnd();

	glPopMatrix();
}

void acBody::Synchronize(){
	Transform2f transform;
	transform.translation.Set(position.x, position.y);
	transform.rotation.Set(rotation);
	shape_ptr->ComputeAABB(&aabb, transform);
}