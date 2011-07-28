#pragma once
#include "acMath.h"

class AIGraphics
{
public:
	AIGraphics(void){
		scaleX = scaleY = 1.0f;
	};

	virtual ~AIGraphics(void){};

	virtual void Update(){};

	float scaleX;
	float scaleY;

	virtual void Draw(const Vec2f& position, float rotation) =0;
	//virtual void Draw(T x, T y, float rotation);

	virtual void setAnchor(float $xRatio, float $yRatio) =0;
	virtual Vec2f& anchor() =0;

	virtual void setWidth(float $w) =0;
	virtual void setHeight(float $h) =0;
	virtual void setSize(float $w, float $h) =0;

	virtual const float width() =0;
	virtual const float height() =0;
};

