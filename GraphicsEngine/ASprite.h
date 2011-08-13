#pragma once
#include "ATextureNode.h"

class ASprite : public ATextureNode
{
public:
	ASprite(void);
	ASprite(const std::string& $fileName);
	ASprite(const std::string& $fileName, const Recti& $rect);
	virtual ~ASprite(void);

	// Draw the image to a specific position and rotation
	void Draw(float x, float y, float z, float rotation);

	// Draw the image using a specific matrix transformation
	void Draw(const Mat4f& mat);

	// Draw the image to a specific position and rotation
	void Draw();

	void CreateVBO(void);

private:
	GLuint _vaoID, _vboID;
};

