#pragma once
#include "acMath.h"

class acBody;
class AImage;

class Tile
{
public:
	Tile(void);
	~Tile(void);

	virtual void Init(const std::string& textureName, const Recti& rect, float hw, float hh);

	virtual void Update();
	virtual void Draw();
	
	virtual Vec2f& position();

	virtual float rotation();
	virtual void setRotation(float $rotation);

	acBody* GetBody();
	void SetBody(acBody* body);

	AImage* image;

protected:
	float _rotation;
	acBody* _body;
};


