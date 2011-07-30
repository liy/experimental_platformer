#include "AImage.h"
#include "ATextureManager.h"
#include "ATexture.h"
#include "acMath.h"

AImage::AImage(void){
	// init tint colour
	colour.Set(1.0f, 1.0f, 1.0f, 1.0f);

	// setup default scale and anchor position
	scale.Set(1.0f, 1.0f);
	anchorRatio.Set(0.5f, 0.5f);
}

AImage::AImage(const std::string& $fileName){
	_texture_sp = ATextureManager::GetInstance()->Get($fileName);

	// Set the rect to be the size of the image
	setRect(0, 0, _texture_sp->contentWidth(), _texture_sp->contentHeight());

	// init tint colour
	colour.Set(1.0f, 1.0f, 1.0f, 1.0f);

	// setup default scale and anchor position
	scale.Set(1.0f, 1.0f);
	anchorRatio.Set(0.5f, 0.5f);
}

AImage::AImage(const std::string& $fileName, const Recti& $rect){
	_texture_sp = ATextureManager::GetInstance()->Get($fileName);

	// assign the texture rectangle.
	setRect($rect);

	// setup default scale and anchor position
	scale.Set(1.0f, 1.0f);
	anchorRatio.Set(0.5f, 0.5f);
}

AImage::~AImage(void)
{
	free(_texCoord);

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

void AImage::SetTexture(const std::string& $fileName, const Recti& $rect){
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

void AImage::SetTexture(const std::string& $fileName){
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

void AImage::Draw(float x, float y, float z, float rotation){
	//bind the texture
	ATextureManager::GetInstance()->Bind(_texture_sp->fileName());

	glPushMatrix();

	// tint the colour
	glColor4f(colour.r, colour.g, colour.b, colour.a);

	// do the transformation
	glTranslatef(x, y, z);//normal position translation transform
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);//rotation transform
	glScalef(scale.x, scale.y, 1.0f);// scale transform
	glTranslatef(-width()*anchorRatio.x, -height()*anchorRatio.y, 0.0f);//anchor translation transform

	glBegin(GL_QUADS);
	glTexCoord2f(_texCoord[0].x, _texCoord[0].y);			glVertex3f(0.0f, 0.0f, z);
	glTexCoord2f(_texCoord[1].x, _texCoord[1].y);			glVertex3f(_rect.width, 0.0f, z);
	glTexCoord2f(_texCoord[2].x, _texCoord[2].y);			glVertex3f(_rect.width, _rect.height, z);
	glTexCoord2f(_texCoord[3].x, _texCoord[3].y);			glVertex3f(0.0f, _rect.height, z);
	glEnd();

	// finished drawing disable texture 2d.
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void AImage::setRect(const Recti& $rect){
	// copy assignment
	_rect = $rect;

	// calculate bottom left of the image in texture coordinate. 
	float x = (float)_rect.x/_texture_sp->width();
	float y = (float)_rect.y/_texture_sp->height();
	// Calculate the the width and height in texture coordinate.
	float w = (float)_rect.width/_texture_sp->width();
	float h = (float)_rect.height/_texture_sp->height();

	// assign the texture coordinate
	_texCoord[0].Set(x, y);
	_texCoord[1].Set(x + w, y);
	_texCoord[2].Set(x + w, y + h);
	_texCoord[3].Set(x, y + h);
}

void AImage::setRect(int $x, int $y, int $width, int $height){
	_rect.x = $x;
	_rect.y = $y;
	_rect.width = $width;
	_rect.height = $height;

	// calculate bottom left of the image in texture coordinate. 
	float x = (float)_rect.x/_texture_sp->width();
	float y = (float)_rect.y/_texture_sp->height();
	// Calculate the the width and height in texture coordinate.
	float w = (float)_rect.width/_texture_sp->width();
	float h = (float)_rect.height/_texture_sp->height();

	// assign the texture coordinate
	_texCoord[0].Set(x, y);
	_texCoord[1].Set(x + w, y);
	_texCoord[2].Set(x + w, y + h);
	_texCoord[3].Set(x, y + h);
}

const Recti& AImage::rect() const{
	return _rect;
}

Vec2f AImage::anchor() const{
	return Vec2f(width()*anchorRatio.x, height()*anchorRatio.y);
}

const std::string& AImage::fileName() const{
	return _texture_sp->fileName();
}

void AImage::setWidth(float $w){
	scale.x = $w/(float)_rect.width;
}

void AImage::setHeight(float $h){
	scale.y = $h/(float)_rect.height;
}

void AImage::setSize(float $w, float $h){
	scale.Set($w/(float)_rect.width, $h/(float)_rect.height);
}

const float AImage::width() const{
	return (float)(_rect.width)*scale.x;
}

const float AImage::height() const{
	return (float)(_rect.height)*scale.y;
}