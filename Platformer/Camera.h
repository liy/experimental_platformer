#pragma once
#include "acMath.h"

class Player;
class Game;

// TODO: refine this class.
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
	void Update(unsigned short delta);

	// Setup the camera before we transform any models.
	void Setup();

	// The position represent the bottom left corner of the camera.
	// You should set anchor to do the shift
	Vec2f position;

	float rotation;

	Game* game;

	// The ratio will be apply to the viewport width and height, and shift gluLookAt eye position and centre position.
	// so camera is actually pointed to the (position + shift).
	Vec2f anchorRatio;

	
	//void SetAnchorRatio(float ratioX, float ratioY);

	Vec2f GetViewportSize() const;

	// lock camera to a actor.
	void Follows(const Player& actor);
	void Unlock();
	const Player* GetLockedTarget();

	void ZoomTo(const Rectf& rect);

protected:
	float currentWidth;
	float currentHeight;
	
	float oWidth;
	float oHeight;

	float scale;

	// You can lock the camera to the target so the camera will always follows to it.
	const Player* lockedTarget;
};

