#pragma once
#include "acMath.h"

class AGraphics
{
public:
	AGraphics(void);
	~AGraphics(void);

	void Update();

	void Draw(const Vec2f& position, float rotation);

	void setAnchor(float $xRatio, float $yRatio);
	Vec2f& anchor();

	void setWidth(float $w);
	void setHeight(float $h);
	void setSize(float $w, float $h);

	const float width();
	const float height();
};

