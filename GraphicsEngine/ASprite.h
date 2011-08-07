#pragma once

#include <iostream>
#include <memory>
#include "AIGraphics.h"

class ATexture;
class ATextureManager;

class ASprite : public AIGraphics
{
public:
	ASprite(void);
	ASprite(const std::string& $fileName);
	ASprite(const std::string& $fileName, const Recti& $rect);
	virtual ~ASprite(void);

	// scale of the image
	Vec2f scale;

	// tint colour of the image
	Vec4f colour;

	// Set anchor ratio internal use only
	Vec2f anchorRatio;

	// update the texture this image is using.
	void SetTexture(const std::string& $fileName, const Recti& $rect);
	// update the texture this image is using.
	void SetTexture(const std::string& $fileName);
	// Get the currently using texture file name.
	const std::string& fileName() const;

	// Draw the image to a specific position and rotation
	virtual void Draw(const Vec3f& position, float rotation){
		Draw(position.x, position.y, position.z, rotation);
	}

	// Draw the image to a specific position and rotation
	virtual void Draw(const Vec2f& position, float z, float rotation){
		Draw(position.x, position.y, z, rotation);
	}
	
	// Draw the image to a specific position and rotation
	void Draw(float x, float y, float z, float rotation);

	void Draw(const Mat4f& mat);

	// Setup texture coordinate
	void setRect(const Recti& $rect);
	// Setup texture coordinate
	void setRect(int $x, int $y, int $width, int $height);
	// Get the texture coordinate
	const Recti& rect() const;

	// Get the anchor position related to the whole texture
	Vec2f anchor() const;

	// Set the width and height of the image. This actually update the scale value of the image.
	void setWidth(float $w);
	void setHeight(float $h);
	void setSize(float $w, float $h);

	// Get the width and height of the image. The width and height is the original size applied scale.
	const float width() const;
	const float height() const;

protected:
	// TODO: make sure copy constructor is doing things correct with the shared pointer.
	// I assume it will simply do a copy with the pointer, and when the cloned image destructor is called, which will NULL the cloned pointer
	// And try to remove the texture from the TextureManager. So it should be fine to use machine generated copy constructor.
	std::tr1::shared_ptr<ATexture> _texture_sp;

	// The actual offset position, width and height, related to the texture resolution.
	Recti _rect;

	// TODO: change the rect into an array of vertices
	Vec3f _vertices[4];

	// Specifying 4 corners in the texture coordinate, in counter clock wise ordering.
	// First vertex is the bottom left, second bottom right, third top right, final one is top left.
	// Note its value is 0.0f ~ 1.0f, texture coordinate.
	Vec2f _texCoord[4];
};

