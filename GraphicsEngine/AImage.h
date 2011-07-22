#pragma once

#include <iostream>
#include <memory>
#include "acMath.h"

class ATexture;
class ATextureManager;
class AQuad;

class AImage
{
public:
	AImage(void);
	AImage(const std::string& $fileName);
	AImage(const std::string& $fileName, const Recti& $rect);
	virtual ~AImage(void);
	
	// Should not be use!!! Too dangerous!! My feeling.
	// You can always achive the smae thing through reference or create brand new AImage
	//AImage(const AImage& $image);

	// DIsallowed, Too dangerous!! My feeling. 
	//AImage& operator=(const AImage& $image);

	void SetTexture(const std::string& $fileName, const Recti& $rect);
	void SetTexture(const std::string& $fileName);

	const std::string& fileName() const;

	void Draw(const Vec2f& position, float rotation);

	void setRect(const Recti& $rect);
	void setRect(int $x, int $y, int $width, int $height);
	const Recti& rect() const;

	// set anchor position
	// TODO: Change the anchor to be the exact position related to the image width and height
	// So in the draw function we do not need to calculate the anchor shift again and agan.
	// Probably we need an varaible to keep track of the anchor translation, and another for record the anchor ratio passed in.
	// Therefore, if we change the texture, the anchor position can be auto updated.
	void setAnchor(float $xRatio, float $yRatio);
	Vec2f& anchor();

	// directly set and get depth is safe
	float depth;

	float scaleX;
	float scaleY;

	float alpha;

	void setWidth(float $w);
	void setHeight(float $h);
	void setSize(float $w, float $h);

	const float width();
	const float height();

	bool tintRed;

	AQuad* debugRect;

protected:
	std::tr1::shared_ptr<ATexture> _texture_sp;

	Vec2f _anchor;
	// anchor ratio internal use only
	Vec2f _anchorRatio;

	Recti _rect;

	float _texOffsetX;
	float _texOffsetY;
	float _texWidth;
	float _texHeight;
};

