#pragma once
class GamepadEventListener
{
public:
	GamepadEventListener(void);
	virtual ~GamepadEventListener(void);

	virtual void Move(float xRatio, float yRatio);
	virtual void Jump();
	virtual void JumpRelease();
	virtual void Stop();

protected:
	bool _jumpRelased;
	unsigned short _jumpConfirmCounter;
};
