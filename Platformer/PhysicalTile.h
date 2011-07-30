#pragma once
#include "ITile.h"

class acBody;
class AImage;

class PhysicalTile: public ITile
{
public:
	PhysicalTile(void);
	~PhysicalTile(void);

	virtual void Init(const std::string& textureName, const Recti& rect, float hw, float hh);

	virtual void Update(unsigned short delta);
	virtual void Draw();

	virtual float rotation() const;
	virtual void SetRotation(float r);

	virtual Vec2f& position();
	virtual void SetPosition(float x, float y);
	virtual void SetPosition(const Vec2f& pos);

	virtual acBody* body();
	virtual void SetBody(acBody* body);

protected:
	AImage* image;
	float _rotation;
	acBody* _body;
};


