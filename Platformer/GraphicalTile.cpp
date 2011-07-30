#include "GraphicalTile.h"
#include "AIGraphics.h"
#include "AAnimation.h"
#include "AImage.h"

GraphicalTile::GraphicalTile(void): _rotation(0.0f)
{
	_position.SetZero();
}

GraphicalTile::GraphicalTile(AImage* img): _rotation(0.0f){
	// copy the image passed in. Since it can be stored in stack
	_graphic_ptr = img;

	_position.SetZero();
}

GraphicalTile::GraphicalTile(AAnimation* ani): _rotation(0.0f){
	// copy the animation passed in
	_graphic_ptr = ani;

	_position.SetZero();
}

GraphicalTile::~GraphicalTile(void)
{
	delete _graphic_ptr;
}

void GraphicalTile::SetAnimation(const AAnimation& ani){
	_graphic_ptr = new AAnimation(ani);
}

void GraphicalTile::SetImage(const AImage& img){
	_graphic_ptr = new AImage(img);
}

void GraphicalTile::Update(unsigned short delta){
	_graphic_ptr->Update(delta);
}

void GraphicalTile::Draw(){
	_graphic_ptr->Draw(_position, 0.0f, _rotation);
}

float GraphicalTile::rotation() const{
	return _rotation;
}

void GraphicalTile::SetRotation(float r){
	_rotation = r;
}

Vec2f& GraphicalTile::position(){
	return _position;
}

void GraphicalTile::SetPosition(float x, float y){
	_position.Set(x, y);
}

void GraphicalTile::SetPosition(const Vec2f& pos){
	_position = pos;
}

AIGraphics* GraphicalTile::graphics(){
	return _graphic_ptr;
}