#pragma once
class GamepadEventListener
{
public:
	GamepadEventListener(void);
	virtual ~GamepadEventListener(void);

	// left stick control, direction control
	virtual void Move(float xRatio, float yRatio);

	// right stick control, zoom and pan camera for testing only
	virtual void UpdateCamera(float xRatio, float yRatio);

	virtual void Jump();
	virtual void JumpRelease();
	virtual void Stop();

	// testing functions
	virtual void LockOn();

protected:
	bool _jumpRelased;
	unsigned short _jumpConfirmCounter;
};
