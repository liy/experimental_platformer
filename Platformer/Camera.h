#pragma once
#include "acMath.h"

class Camera
{
public:
	Camera(float w, float h);
	~Camera(void);

	// Smoothly tween camera to the target position.
	void TweenTo(const Vec2f& tp);

	// Directly jump the camera to the position
	void MoveTo(const Vec2f& tp);

	// Move the camera by a displacement vector.
	void Move(const Vec2f& dis);

	// Resize the camera's viewport 
	void ResizeViewport(float w, float h);

	// Setup the camera before we transform any models.
	void Setup();

	// The position represent the bottom left corner of the camera.
	// You should set anchor to do the shift
	Vec2f position;

	Vec2f anchorRatio;

protected:
	float width;
	float height;

	Vec2f anchorShift;
};

