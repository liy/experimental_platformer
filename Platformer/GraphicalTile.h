#pragma once
#include "ITile.h"

class ASprite;
class AAnimation;
class AIGraphics;

// No user interaction available. Purely graphical representation
class GraphicalTile: public ITile
{
public:
	GraphicalTile(void);
	GraphicalTile(ASprite* image);
	GraphicalTile(AAnimation* ani);
	~GraphicalTile(void);

	virtual void SetSprite(const ASprite& img);
	virtual void SetAnimation(const AAnimation& ani);

	virtual void Update(unsigned short delta);
	virtual void Draw();

	virtual float rotation() const;
	virtual void SetRotation(float r);

	virtual const Vec2f& position() const;
	virtual void SetPosition(float x, float y);
	virtual void SetPosition(const Vec2f& pos);

	virtual AIGraphics* graphics();
protected:
	AIGraphics* _graphics_ptr;

	Vec2f _position;
	float _rotation;
};

