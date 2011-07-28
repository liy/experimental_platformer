#include "AImage.h"
#include "ATextureManager.h"
#include "ATexture.h"
#include "AQuad.h"
#include "acMath.h"

AImage::AImage(void): depth(0.0f), scaleX(1.0f), scaleY(1.0f), alpha(1.0f), tintRed(false)
{
	_anchor.x = 0.0f;
	_anchor.y = 0.0f;
	_anchorRatio.x = 0.5f;
	_anchorRatio.y = 0.5f;

	debugRect = new AQuad();
}

AImage::AImage(const std::string& $fileName): depth(0.0f), scaleX(1.0f), scaleY(1.0f), alpha(1.0f), tintRed(false)
{
	_texture_sp = ATextureManager::GetInstance()->Get($fileName);

	// Set the rect to be the size of the image
	setRect(0, 0, _texture_sp->contentWidth(), _texture_sp->contentHeight());

	_anchor.x = 0.0f;
	_anchor.y = 0.0f;
	_anchorRatio.x = 0.5f;
	_anchorRatio.y = 0.5f;
	
	debugRect = new AQuad();
}

AImage::AImage(const std::string& $fileName, const Recti& $rect): depth(0.0f), scaleX(1.0f), scaleY(1.0f), alpha(1.0f), tintRed(false)
{
	_texture_sp = ATextureManager::GetInstance()->Get($fileName);

	setRect($rect);

	_anchor.x = 0.0f;
	_anchor.y = 0.0f;
	_anchorRatio.x = 0.5f;
	_anchorRatio.y = 0.5f;
	
	debugRect = new AQuad();
}

/*
// Programmer should not use Copy constructor to copy any AImage instance.
// They have to manually create a new instance, or just using the reference
AImage::AImage(const AImage& $image) : _fileName($image._fileName)
{
	std::cout << "copy constructor called\n";
	_texture_sp = ATextureManager::GetInstance()->Get($image._fileName);

	// Set the rect to be the size of the image
	setRect($image._rect); 
}
*/

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

/*
// Programmer should not use assignment operator to copy any AImage instance.
// They have to manually create a new instance, or just using the reference
AImage& AImage::operator=(const AImage& $image){
	std::cout << "assignment operator called\n";
	 _fileName = $image._fileName;
	_texture_sp = ATextureManager::GetInstance()->Get($image._fileName);

	// Set the rect to be the size of the image
	setRect($image._rect); 
	return *this;
}
*/


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
	setRect($rect);
	// update the anchor translation
	setAnchor(_anchorRatio.x, _anchorRatio.y);
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
	setRect(0, 0, _texture_sp->contentWidth(), _texture_sp->contentHeight());
	// update the anchor translation
	setAnchor(_anchorRatio.x, _anchorRatio.y);
}

void AImage::Draw(const Vec2f& position, float rotation){
	//std::cout << "draw: " << _texture_sp->fileName() << "\n";
	glPushMatrix();

	//bind the texture
	ATextureManager::GetInstance()->Bind(_texture_sp->fileName());

	// do anchor shift.
	_anchor.x = -width()*_anchorRatio.x;
	_anchor.y = -height()*_anchorRatio.y;

	glTranslatef(position.x, position.y, 0.0f);//normal position translation transform
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);//rotation transform
	glScalef(scaleX, scaleY, 0.0f);// scale transform
	glTranslatef(_anchor.x, _anchor.y, 0.0f);//anchor translation transform
	
	
	if(!tintRed){
		glColor4f(1.0f, 1.0f, 1.0f, alpha);
	}
	else{
		glColor4f(1.0f, 0.0f, 0.0f, alpha);
	}
	
	glBegin(GL_QUADS);
		glTexCoord2f(_texOffsetX, _texOffsetY);								glVertex3f(0.0f, 0.0f, depth);
		glTexCoord2f(_texWidth + _texOffsetX, _texOffsetY);					glVertex3f(_rect.width, 0.0f, depth);
		glTexCoord2f(_texWidth + _texOffsetX, _texHeight + _texOffsetY);	glVertex3f(_rect.width, _rect.height, depth);
		glTexCoord2f(_texOffsetX, _texHeight + _texOffsetY);				glVertex3f(0.0f, _rect.height, depth);
	glEnd();

	// finsihed drawing disable texture 2d.
	glDisable(GL_TEXTURE_2D);
	

	glPopMatrix();
}

void AImage::setRect(const Recti& $rect){
	_rect = $rect;

	_texWidth = (float)_rect.width/_texture_sp->width();
	_texHeight = (float)_rect.height/_texture_sp->height();
	_texOffsetX = (float)_rect.x/_texture_sp->width();
	_texOffsetY = (float)_rect.y/_texture_sp->height();
}

void AImage::setRect(int $x, int $y, int $width, int $height){
	_rect.x = $x;
	_rect.y = $y;
	_rect.width = $width;
	_rect.height = $height;

	_texWidth = (float)_rect.width/_texture_sp->width();
	_texHeight = (float)_rect.height/_texture_sp->height();
	_texOffsetX = (float)_rect.x/_texture_sp->width();
	_texOffsetY = (float)_rect.y/_texture_sp->height();
}

const Recti& AImage::rect() const{
	return _rect;
}

void AImage::setAnchor(float $xRatio, float $yRatio){
	// record the ratio, so if we dynamically change texture, we can update the anchor position
	// according to the old ratio
	_anchorRatio.x = $xRatio;
	_anchorRatio.y = $yRatio;
	// record the anchor translation, so we do not need to calculate them in draw function.
	// _anchor.x = -width()*_anchorRatio.x;
	// _anchor.y = -height()*_anchorRatio.y;
}

Vec2f& AImage::anchor(){
	return _anchor;
}

const std::string& AImage::fileName() const{
	return _texture_sp->fileName();
}

void AImage::setWidth(float $w){
	scaleX = $w/(float)_rect.width;
}

void AImage::setHeight(float $h){
	scaleY = $h/(float)_rect.height;
}

void AImage::setSize(float $w, float $h){
	setWidth($w);
	setHeight($h);
}

const float AImage::width(){
	return (float)(_rect.width)*scaleX;
}

const float AImage::height(){
	return (float)(_rect.height)*scaleY;
}