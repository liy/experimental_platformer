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

	// init tint colour
	colour.Set(1.0f, 1.0f, 1.0f, 1.0f);

	// setup default scale and anchor position
	scale.Set(1.0f, 1.0f);
	anchorRatio.Set(0.5f, 0.5f);
}

AImage::~AImage(void)
{
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
	glPushMatrix();

	//bind the texture
	ATextureManager::GetInstance()->Bind(_texture_sp->fileName());

	// tint the colour
	glColor4f(colour.r, colour.g, colour.b, colour.a);

	// do the transformation
	glTranslatef(x, y, z);//normal position translation transform
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);//rotation transform
	glScalef(scale.x, scale.y, 1.0f);// scale transform
	glTranslatef(-width()*anchorRatio.x, -height()*anchorRatio.y, 0.0f);//anchor translation transform

	glBegin(GL_QUADS);
	glTexCoord2f(_texCoord.x, _texCoord.y);											glVertex3f(0.0f, 0.0f, z);
	glTexCoord2f(_texCoord.x + _texCoord.width, _texCoord.y);						glVertex3f(_rect.width, 0.0f, z);
	glTexCoord2f(_texCoord.x + _texCoord.width, _texCoord.y + _texCoord.height);	glVertex3f(_rect.width, _rect.height, z);
	glTexCoord2f(_texCoord.x, _texCoord.y + _texCoord.height);						glVertex3f(0.0f, _rect.height, z);
	glEnd();

	// finished drawing disable texture 2d.
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void AImage::setRect(const Recti& $rect){
	// copy assignment
	_rect = $rect;

	_texCoord.width = (float)_rect.width/_texture_sp->width();
	_texCoord.height = (float)_rect.height/_texture_sp->height();
	_texCoord.x = (float)_rect.x/_texture_sp->width();
	_texCoord.y = (float)_rect.y/_texture_sp->height();
}

void AImage::setRect(int $x, int $y, int $width, int $height){
	_rect.x = $x;
	_rect.y = $y;
	_rect.width = $width;
	_rect.height = $height;

	_texCoord.width = (float)_rect.width/_texture_sp->width();
	_texCoord.height = (float)_rect.height/_texture_sp->height();
	_texCoord.x = (float)_rect.x/_texture_sp->width();
	_texCoord.y = (float)_rect.y/_texture_sp->height();
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