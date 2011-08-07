#include "ASprite.h"
#include "ATextureManager.h"
#include "ATexture.h"
#include "acMath.h"

ASprite::ASprite(void){
	// init tint colour
	colour.Set(1.0f, 1.0f, 1.0f, 1.0f);

	// setup default scale and anchor position
	scale.Set(1.0f, 1.0f);
	anchorRatio.Set(0.5f, 0.5f);

	horizontalFlip = false;
}

ASprite::ASprite(const std::string& $fileName){
	_texture_sp = ATextureManager::GetInstance()->Get($fileName);

	// Set the rect to be the size of the image
	setRect(0, 0, _texture_sp->contentWidth(), _texture_sp->contentHeight());

	// init tint colour
	colour.Set(1.0f, 1.0f, 1.0f, 1.0f);

	// setup default scale and anchor position
	scale.Set(1.0f, 1.0f);
	anchorRatio.Set(0.5f, 0.5f);

	horizontalFlip = false;
}

ASprite::ASprite(const std::string& $fileName, const Recti& $rect){
	_texture_sp = ATextureManager::GetInstance()->Get($fileName);

	// assign the texture rectangle.
	setRect($rect);

	// init tint colour
	colour.Set(1.0f, 1.0f, 1.0f, 1.0f);

	// setup default scale and anchor position
	scale.Set(1.0f, 1.0f);
	anchorRatio.Set(0.5f, 0.5f);

	horizontalFlip = false;
}

ASprite::~ASprite(void)
{
	// FIXME how to free the image texture coordinate????

	// keep a copy record of the filename, to remove the 
	std::string fileName = _texture_sp->fileName();
	std::cout << "AImage["<< fileName <<"] destroy!\n";
	// Null the reference, so we can try to remove th texture
	_texture_sp = NULL;
	// Try to remove the using texture from the memory.
	// If the reference count is 1.(1 reference count is maintained by the map). Then we remove it from the memory.
	// So programmer will not need to manually  remove texture from the memory
	ATextureManager::GetInstance()->Remove(fileName);
}

void ASprite::SetTexture(const std::string& $fileName, const Recti& $rect){
	// Dynamically change texture, we have to try to remove the previous texture it was using.
	if(_texture_sp != NULL && _texture_sp->fileName() != $fileName){
		std::string fileName = _texture_sp->fileName();
		// Null the reference, so we can try to remove th texture
		_texture_sp = NULL;
		// Try to remove the using texture from the memory.
		ATextureManager::GetInstance()->Remove(fileName);
	}
	// update to the new texture
	_texture_sp = ATextureManager::GetInstance()->Get($fileName);

	// update texture coordinate
	setRect($rect);

	// setup default scale and anchor position
	scale.Set(1.0f, 1.0f);
	anchorRatio.Set(0.5f, 0.5f);
}

void ASprite::SetTexture(const std::string& $fileName){
	// Dynamically change texture, we have to try to remove the previous texture it was using.
	if(_texture_sp != NULL && _texture_sp->fileName() != $fileName){
		std::string fileName = _texture_sp->fileName();
		// Null the reference, so we can try to remove th texture
		_texture_sp = NULL;
		// Try to remove the using texture from the memory.
		ATextureManager::GetInstance()->Remove(fileName);
	}
	// update to the new texture
	_texture_sp = ATextureManager::GetInstance()->Get($fileName);
	// update texture coordinate
	setRect(0, 0, _texture_sp->contentWidth(), _texture_sp->contentHeight());
	
	// setup default scale and anchor position
	scale.Set(1.0f, 1.0f);
	anchorRatio.Set(0.5f, 0.5f);
}

void ASprite::Draw(float x, float y, float z, float rotation){
	//bind the texture
	ATextureManager::GetInstance()->Bind(_texture_sp->fileName());

	glPushMatrix();

	// tint the colour
	glColor4f(colour.r, colour.g, colour.b, colour.a);

	
	// do the transformation
	glTranslatef(x, y, z);//normal position translation transform
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);//rotation transform
	glScalef(scale.x, scale.y, 1.0f);// scale transform

	// TODO: multiply the transformation matrix.
	glMultMatrixf(_transform);

	glTranslatef(-width()*anchorRatio.x, -height()*anchorRatio.y, 0.0f);//anchor translation transform

	

	glBegin(GL_QUADS);
	if(!horizontalFlip){
		glTexCoord2f(_texCoord[0].u, _texCoord[0].v);			glVertex3f(0.0f, 0.0f, z);
		glTexCoord2f(_texCoord[1].u, _texCoord[1].v);			glVertex3f(_rect.width, 0.0f, z);
		glTexCoord2f(_texCoord[2].u, _texCoord[2].v);			glVertex3f(_rect.width, _rect.height, z);
		glTexCoord2f(_texCoord[3].u, _texCoord[3].v);			glVertex3f(0.0f, _rect.height, z);
	}
	else{
		glTexCoord2f(_texCoord[1].u, _texCoord[1].v);			glVertex3f(0.0f, 0.0f, z);
		glTexCoord2f(_texCoord[0].u, _texCoord[0].v);			glVertex3f(_rect.width, 0.0f, z);
		glTexCoord2f(_texCoord[3].u, _texCoord[3].v);			glVertex3f(_rect.width, _rect.height, z);
		glTexCoord2f(_texCoord[2].u, _texCoord[2].v);			glVertex3f(0.0f, _rect.height, z);
	}
	glEnd();

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0f, 0.0f, z);
	glVertex3f(32.0f, 0.0f, z);
	glVertex3f(32.0f, 32.0f, z);
	glVertex3f(0.0f, 32.0f, z);
	glEnd();

	// finished drawing disable texture 2d.
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void ASprite::Draw(const Mat4f& mat){
	//bind the texture
	ATextureManager::GetInstance()->Bind(_texture_sp->fileName());

	glPushMatrix();

	// tint the colour
	glColor4f(colour.r, colour.g, colour.b, colour.a);

	// since there's no transformation before this line, we can safely  directly load the input matrix
	glLoadMatrixf(mat);
	// The anchor translation transform will be concatenated
	// inputMatrix * anchorTransMatrix * vertices
	glTranslatef(-width()*anchorRatio.x, -height()*anchorRatio.y, 0.0f);

	glBegin(GL_QUADS);
	if(!horizontalFlip){
		glTexCoord2f(_texCoord[0].u, _texCoord[0].v);			glVertex3f(0.0f, 0.0f, 0.0f);
		glTexCoord2f(_texCoord[1].u, _texCoord[1].v);			glVertex3f(_rect.width, 0.0f, 0.0f);
		glTexCoord2f(_texCoord[2].u, _texCoord[2].v);			glVertex3f(_rect.width, _rect.height, 0.0f);
		glTexCoord2f(_texCoord[3].u, _texCoord[3].v);			glVertex3f(0.0f, _rect.height, 0.0f);
	}
	else{
		glTexCoord2f(_texCoord[1].u, _texCoord[1].v);			glVertex3f(0.0f, 0.0f, 0.0f);
		glTexCoord2f(_texCoord[0].u, _texCoord[0].v);			glVertex3f(_rect.width, 0.0f, 0.0f);
		glTexCoord2f(_texCoord[3].u, _texCoord[3].v);			glVertex3f(_rect.width, _rect.height, 0.0f);
		glTexCoord2f(_texCoord[2].u, _texCoord[2].v);			glVertex3f(0.0f, _rect.height, 0.0f);
	}
	glEnd();

	// finished drawing disable texture 2d.
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void ASprite::setRect(const Recti& $rect){
	// copy assignment
	_rect = $rect;

	// calculate bottom left of the image in texture coordinate. 
	float u = (float)_rect.x/_texture_sp->width();
	float v = (float)_rect.y/_texture_sp->height();
	// Calculate the the width and height in texture coordinate.
	float w = (float)_rect.width/_texture_sp->width();
	float h = (float)_rect.height/_texture_sp->height();

	// assign the texture coordinate
	_texCoord[0].Set(u, v);
	_texCoord[1].Set(u + w, v);
	_texCoord[2].Set(u + w, v + h);
	_texCoord[3].Set(u, v + h);
}

void ASprite::setRect(int $x, int $y, int $width, int $height){
	_rect.x = $x;
	_rect.y = $y;
	_rect.width = $width;
	_rect.height = $height;

	// calculate bottom left of the image in texture coordinate. 
	float u = (float)_rect.x/_texture_sp->width();
	float v = (float)_rect.y/_texture_sp->height();
	// Calculate the the width and height in texture coordinate.
	float w = (float)_rect.width/_texture_sp->width();
	float h = (float)_rect.height/_texture_sp->height();

	// assign the texture coordinate
	_texCoord[0].Set(u, v);
	_texCoord[1].Set(u + w, v);
	_texCoord[2].Set(u + w, v + h);
	_texCoord[3].Set(u, v + h);
}

const Recti& ASprite::rect() const{
	return _rect;
}

Vec2f ASprite::anchor() const{
	return Vec2f(width()*anchorRatio.x, height()*anchorRatio.y);
}

const std::string& ASprite::fileName() const{
	return _texture_sp->fileName();
}

void ASprite::setWidth(float $w){
	scale.x = $w/(float)_rect.width;
}

void ASprite::setHeight(float $h){
	scale.y = $h/(float)_rect.height;
}

void ASprite::setSize(float $w, float $h){
	scale.Set($w/(float)_rect.width, $h/(float)_rect.height);
}

const float ASprite::width() const{
	return (float)(_rect.width)*scale.x;
}

const float ASprite::height() const{
	return (float)(_rect.height)*scale.y;
}