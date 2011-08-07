#pragma once
#include "acMath.h"

class AIGraphics
{
public:
	AIGraphics(void){
		// init tint colour
		colour.Set(1.0f, 1.0f, 1.0f, 1.0f);

		// setup default scale and anchor position
		scale.Set(1.0f, 1.0f);
		anchorRatio.Set(0.5f, 0.5f);
		horizontalFlip = false;
	};

	virtual ~AIGraphics(void){};

	// scale of the image
	Vec2f scale;

	// tint colour of the image
	Vec4f colour;

	// Set anchor ratio internal use only
	Vec2f anchorRatio;

	bool horizontalFlip;

	// Update the graphic, this probably only apply to the animation. For static image, this function does nothing
	virtual void Update(unsigned short delta) {};

	// Draw the image to a specific position and rotation
	virtual void Draw(const Vec3f& position, float rotation){
		Draw(position.x, position.y, position.z, rotation);
	}

	// Draw the image to a specific position and rotation
	virtual void Draw(const Vec2f& position, float z, float rotation){
		Draw(position.x, position.y, z, rotation);
	}

	// Draw the image to a specific position and rotation
	virtual void Draw(float x, float y, float z, float rotation) =0;

	// Draw the image using a specific matrix transformation
	virtual void Draw(const Mat4f& mat) =0;

	// Get the anchor position related to the whole texture
	virtual Vec2f anchor() const =0;

	// Set the width of the image. This actually update the scale x value of the image.
	virtual void setWidth(float $w) =0;

	// Set the height of the image. This actually update the scale y value of the image.
	virtual void setHeight(float $h) =0;

	// Set the width and height of the image. This actually update the scale values of the image.
	virtual void setSize(float $w, float $h) =0;

	// Get the width of the image. The width is the original width applied scale x.
	virtual const float width() const =0;

	// Get the height of the image. The height is the original height applied scale y.
	virtual const float height() const =0;
};

