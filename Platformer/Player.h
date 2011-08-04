#pragma once
#include "GamepadEventListener.h"
#include "Actor.h"

class Player : public GamepadEventListener, public Actor
{
public:
	Player(Scene* scene);
	Player(Scene* scene, AAnimation* ani, acBody* body);
	~Player(void);

	void Update(unsigned short deltaTime);

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

