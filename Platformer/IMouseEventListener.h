#pragma once
class IMouseEventListener
{
public:
	virtual void MouseDownHandler(short x, short y)=0;
	virtual void MouseMoveHandler(short x, short y)=0;
};

