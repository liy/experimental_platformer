#pragma once
#include "GamepadEventListener.h"

class Scene;
class AAnimation;
class acBody;

// forward declaration of floating vector2
template<class T> class Vec2;
typedef class Vec2<float> Vec2f;

// TODO:  Add jump and walk velocity, they will be combined together to form the body's velocity.
class Actor : public GamepadEventListener
{
public:
	Actor(Scene* scene);
	Actor(Scene* scene, acBody* body);
	~Actor(void);

	// TODO: testing... scene pointer
	Scene* scene_ptr;

	// image pointer, dynamic memory allocation is needed
	AAnimation* animation_ptr;

	// rigid body, dynamic memory allocation is needed
	acBody* body_ptr;

	const Vec2f& Actor::position() const;
	void SetPosition(const Vec2f& p);

	void Update(unsigned short deltaTime);
	void Draw();

	void Land();
	void GrabWall(bool grabbed);

	void Move(float xRatio, float yRatio);
	void Jump();
	void JumpRelease();
	void Stop();
protected:
	bool _floating;
	bool _wallGrabbed;

	// When actor start falling or jumping, we allow a tiny tolerance until  floating is really set to true. In mini-seconds
	static const unsigned short FLOATING_TOLERENCE = 80;

	// indicate the time duration when actor is in floating mode.
	// I don't think the actor can float for 1 minutes..... so I use short here.
	unsigned short _floatingTimer;
};

