#include "Camera.h"
#include <GL\glut.h>
#include <GL\GL.h>

Camera::Camera(float w, float h)
{
	width = w;
	height = h;

	position.SetZero();
	anchorRatio.Set(0.5f, 0.5f);
	anchorShift.Set(-width * anchorRatio.x, -height * anchorRatio.y);
}


Camera::~Camera(void)
{

}

// Resize the camera's viewport 
void Camera::ResizeViewport(float w, float h){
	width = w;
	height = h;

	anchorShift.Set(-width * anchorRatio.x, -height * anchorRatio.y);
}

void Camera::TweenTo(const Vec2f& tp){
	// delta vector.
	Vec2f dis = (tp - position);
	if(dis.Len2() < 1.0f){
		MoveTo(tp);
	}
	else{
		Move(dis*0.1);
	}
}

void Camera::MoveTo(const Vec2f& p){
	position = p;
}

void Camera::Move(const Vec2f& dis){
	position += dis;
}

void Camera::Setup(){
	// for now hard code the camera's z position
	gluLookAt(position.x + anchorShift.x, position.y + anchorShift.y, 1.0f,
		position.x + anchorShift.x, position.y + anchorShift.y, 0.0f,
		0.0f, 1.0f, 0.0f);

	// TODO: apply zoom effect after gluLookAt statement.
	// The actual zoom matrix is multiply to the vertices before camera gluLookAt's view matrix.
}