#include "PhysicalTile.h"
#include "acPolygonShape.h"
#include "acBody.h"
#include "ASprite.h"
#include "AAnimation.h"

PhysicalTile::PhysicalTile(void):_rotation(0.0f)
{
	
}


PhysicalTile::~PhysicalTile(void)
{

}

void PhysicalTile::SetAnimation(AAnimation* ani){
	_graphics_ptr = ani;
}

void PhysicalTile::SetSprite(ASprite* img){
	_graphics_ptr = img;
}

void PhysicalTile::CreateBody(float hw, float hh){
	acPolygonShape* boxShape = new acPolygonShape();
	boxShape->SetAsBox(hw, hh);
	_body = new acBody();
	_body->SetShape(boxShape);
}

void PhysicalTile::SetBody(acBody* body){
	_body = body;
}

acBody* PhysicalTile::body(){
	return _body;
}

void PhysicalTile::Update(unsigned short delta){
	_body->Synchronize();

	_graphics_ptr->Update(delta);
}
	
void PhysicalTile::Draw(){
	//_body->DrawAABB(0.0f, 0.3f, 0.1f);
	_graphics_ptr->Draw(_body->position.x, _body->position.y, 0.0f, _rotation);
}

float PhysicalTile::rotation() const{
	//return _rotation;
	return _body->rotation;
}

void PhysicalTile::SetRotation(float $rotation){
	_body->rotation = $rotation;
}

void PhysicalTile::SetPosition(float x, float y){
	_body->position.Set(x, y);
}

void PhysicalTile::SetPosition(const Vec2f& pos){
	_body->position = pos;
}

const Vec2f& PhysicalTile::position() const{
	return _body->position;
}

