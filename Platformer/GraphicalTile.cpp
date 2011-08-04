#include "GraphicalTile.h"
#include "AIGraphics.h"
#include "AAnimation.h"
#include "ASprite.h"

GraphicalTile::GraphicalTile(void): _rotation(0.0f)
{
	_position.SetZero();
}

// TODO: pass a pointer? or pass by reference then call copy constructor?
GraphicalTile::GraphicalTile(ASprite* img): _rotation(0.0f){
	// copy the image passed in. Since it can be stored in stack
	_graphics_ptr = img;

	_position.SetZero();
}

// TODO: pass a pointer? or pass by reference then call copy constructor?
GraphicalTile::GraphicalTile(AAnimation* ani): _rotation(0.0f){
	// copy the animation passed in
	_graphics_ptr = ani;

	_position.SetZero();
}

GraphicalTile::~GraphicalTile(void)
{
	delete _graphics_ptr;
	_graphics_ptr = NULL;
}

// TODO: pass a pointer? or pass by reference then call copy constructor?
void GraphicalTile::SetAnimation(const AAnimation& ani){
	_graphics_ptr = new AAnimation(ani);
}

// TODO: pass a pointer? or pass by reference then call copy constructor?
void GraphicalTile::SetSprite(const ASprite& img){
	_graphics_ptr = new ASprite(img);
}

void GraphicalTile::Update(unsigned short delta){
	_graphics_ptr->Update(delta);
}

void GraphicalTile::Draw(){
	_graphics_ptr->Draw(_position, 0.0f, _rotation);
}

float GraphicalTile::rotation() const{
	return _rotation;
}

void GraphicalTile::SetRotation(float r){
	_rotation = r;
}

const Vec2f& GraphicalTile::position() const{
	return _position;
}

void GraphicalTile::SetPosition(float x, float y){
	_position.Set(x, y);
}

void GraphicalTile::SetPosition(const Vec2f& pos){
	_position = pos;
}

AIGraphics* GraphicalTile::graphics(){
	return _graphics_ptr;
}