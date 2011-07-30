#pragma once
#include "acMath.h"

class AIGraphics;

// Tiles can be mainly separated into two types
//	1. Normal graphical tiles, only a visual representation. There is not user interaction or physical collision detection.  e.g., background, houses, flowers, grass, etc.
//       2. Tiles which participate in the user interaction, collision detection. e.g., grounding tiles, walls which user can hit.
class ITile
{
public:
	ITile(void){};
	~ITile(void){};

	virtual void Update(unsigned short delta){};
	virtual void Draw() =0;

	virtual float rotation() const =0;
	virtual void SetRotation(float r) =0;

	virtual Vec2f& position() =0;
	virtual void SetPosition(float x, float y) =0;
	virtual void SetPosition(const Vec2f& pos) =0;
};
