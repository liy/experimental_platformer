#include "Camera.h"
#include <GL\glut.h>
#include <GL\GL.h>
#include "Actor.h"

Camera::Camera()
{
	
}


Camera::~Camera(void)
{

}

void Camera::Init(float viewportWidth, float viewportHeight, float anchorRatioX, float anchorRatioY){
	oWidth = currentWidth = viewportWidth;
	oHeight = currentHeight = viewportHeight;

	rotation = 0.0f;

	position.SetZero();

	SetAnchorRatio(anchorRatioX, anchorRatioY);

	scale = 1.0f;

	lockedTarget = NULL;
}

void Camera::SetAnchorRatio(float ratioX, float ratioY){
	anchorRatio.Set(ratioX, ratioY);
	anchorShift.Set(-currentWidth * anchorRatio.x, -currentHeight * anchorRatio.y);
}

// Resize the camera's viewport 
void Camera::ResizeViewport(float w, float h){
	currentWidth = w;
	currentHeight = h;

	scale = oWidth/currentWidth;

	SetAnchorRatio(anchorRatio.x, anchorRatio.y);
}

void Camera::TweenTo(const Vec2f& tp){
	// delta vector.
	Vec2f dis = (tp - position);
	if(dis.Len2() < 1.0f/scale){
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

void Camera::Update(){
	if(lockedTarget != NULL){
		TweenTo(lockedTarget->GetPosition());
	}
}

void Camera::Setup(){
	// cuztomized camera transformation.
	/*
	glScalef(scale, scale, 1.0f);

	glTranslatef(-anchorShift.x, -anchorShift.y, 0.0f);
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);
	
	glTranslatef(-position.x, -position.y, 0.0f);
	*/

	// glutlookat, without rotation.
	/*
	glScalef(scale, scale, 1.0f);
	
	// for now hard code the camera's z position
	gluLookAt(position.x + anchorShift.x, position.y + anchorShift.y, 1.0f,
		position.x + anchorShift.x, position.y + anchorShift.y, 0.0f,
		0.0f, 1.0f, 0.0f);
	*/

	// glulookat with rotation. Needs to seperate the shift translation.
	// scale, related the shifted position.
	glScalef(scale, scale, 1.0f);
	// do further shift translation.
	glTranslatef(-anchorShift.x, -anchorShift.y, 0.0f);
	// rotate about the position
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);
	// move the camera to specified positon, ready for rotation.
	gluLookAt(position.x, position.y, 1.0f,
		position.x, position.y, 0.0f,
		0.0f, 1.0f, 0.0f);
}

void Camera::LockOn(const Actor& actor){
	lockedTarget = &actor;
}

void Camera::Unlock(){
	lockedTarget = NULL;
}

const Actor* Camera::GetLockedTarget(){
	return lockedTarget;
}

Vec2f Camera::GetViewportSize() const{
	return Vec2f(currentWidth, currentHeight);
}