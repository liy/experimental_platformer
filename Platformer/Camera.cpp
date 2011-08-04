#include "Camera.h"
#include <GL\glut.h>
#include <GL\GL.h>
#include "Player.h"
#include "Game.h"

Camera::Camera(Game* gm): game(gm)
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

	scale = 1.0f;

	anchorRatio.Set(anchorRatioX, anchorRatioY);

	lockedTarget = NULL;
}

// Resize the camera's viewport 
void Camera::ResizeViewport(float w, float h){
	currentWidth = w;
	currentHeight = h;

	scale = oWidth/currentWidth;
}

void Camera::ZoomTo(const Rectf& rect){
	float targetScale = oHeight/rect.height;
	scale += (targetScale - scale) * 0.0001f;

	currentWidth = oWidth*scale;
	currentHeight = oHeight*scale;
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

void Camera::Update(unsigned short delta){
	if(lockedTarget != NULL){
		TweenTo(lockedTarget->position());
	}
}

void Camera::Setup(){
	// glulookat with rotation. Needs to separate the shift translation.
	// scale, related the shifted position.
	glScalef(scale, scale, 1.0f);
	// do further shift translation.
	glTranslatef(currentWidth * anchorRatio.x, currentHeight * anchorRatio.y, 0.0f);
	// rotate about the position
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);
	// move the camera to specified position, ready for rotation.
	gluLookAt(position.x, position.y, 1.0f,
		position.x, position.y, 0.0f,
		0.0f, 1.0f, 0.0f);
}

void Camera::Follows(const Player& actor){
	lockedTarget = &actor;
}

void Camera::Unlock(){
	lockedTarget = NULL;
}

const Player* Camera::GetLockedTarget(){
	return lockedTarget;
}

Vec2f Camera::GetViewportSize() const{
	return Vec2f(currentWidth, currentHeight);
}