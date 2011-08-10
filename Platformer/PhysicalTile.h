#pragma once
#include "ITile.h"

class acBody;
class ASprite;
class AAnimation;

class PhysicalTile: public ITile
{
public:
	PhysicalTile(void);
	~PhysicalTile(void);

	virtual void Update(unsigned short delta);
	virtual void Draw();

	virtual void SetSprite(ASprite* img);
	virtual void SetAnimation(AAnimation* ani);

	virtual void SetBody(acBody* body);
	virtual void CreateBody(float hw, float hh);
	virtual acBody* body();

	virtual float rotation() const;
	virtual void SetRotation(float r);

	virtual const Vec2f& position() const;
	virtual void SetPosition(float x, float y);
	virtual void SetPosition(const Vec2f& pos);

protected:
	ATextureNode* _graphics_ptr;
	float _rotation;
	acBody* _body;
};


