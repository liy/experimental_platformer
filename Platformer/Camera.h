#pragma once
#include "acMath.h"

class Actor;
class Game;

class Camera
{
public:
	Camera(Game* gm);
	~Camera(void);

	void Init(float viewportWidth, float viewportHeight, float anchorRatioX = 0.5f, float anchorRatioY = 0.5f);

	// Smoothly tween camera to the target position.
	void TweenTo(const Vec2f& tp);

	// Directly jump the camera to the position
	void MoveTo(const Vec2f& tp);

	// Move the camera by a displacement vector.
	void Move(const Vec2f& dis);

	// Resize the camera's viewport 
	void ResizeViewport(float w, float h);

	// update camera, such as, position, rotation...
	void Update();

	// Setup the camera before we transform any models.
	void Setup();

	// The position represent the bottom left corner of the camera.
	// You should set anchor to do the shift
	Vec2f position;

	float rotation;

	// The ratio will be apply to the viewport width and height, and shift gluLookAt eye position and centre position.
	// so camera is actually pointed to the (position + shift).
	void SetAnchorRatio(float ratioX, float ratioY);

	Vec2f GetViewportSize() const;

	// lock camera to a actor.
	void Follows(const Actor& actor);
	void Unlock();
	const Actor* GetLockedTarget();

	void ZoomTo(const Rectf& rect);

protected:
	float currentWidth;
	float currentHeight;
	
	float oWidth;
	float oHeight;

	float scale;

	Vec2f anchorRatio;
	Vec2f anchorShift;

	// You can lock the camera to the target so the camera will always follows to it.
	const Actor* lockedTarget;

	Game* game;
};

