#pragma once
#include "acMath.h"

class acBody;

class Tile
{
public:
	Tile(void);
	~Tile(void);

	virtual void Update();
	virtual void Draw();
	
	virtual Vec2f& position();

	virtual float rotation();
	virtual void setRotation(float $rotation);

	acBody* GetBody();
	void SetBody(acBody* body);


protected:
	float _rotation;
	acBody* _body;
};


