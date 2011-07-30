#include "PhysicalTile.h"
#include "acPolygonShape.h"
#include "acBody.h"
#include "AImage.h"

PhysicalTile::PhysicalTile(void):_rotation(0.0f)
{
	
}


PhysicalTile::~PhysicalTile(void)
{

}

void PhysicalTile::Init(const std::string& textureName, const Recti& rect, float hw, float hh){
	acPolygonShape* boxShape = new acPolygonShape();
	boxShape->SetAsBox(hw, hh);
	_body = new acBody();
	_body->SetShape(boxShape);

	image = new AImage(textureName);
	image->setRect(rect);
}

void PhysicalTile::SetBody(acBody* body){
	_body = body;
}

acBody* PhysicalTile::body(){
	return _body;
}

void PhysicalTile::Update(unsigned short delta){
	_body->Synchronize();
}
	
void PhysicalTile::Draw(){
	_body->DrawAABB(0.0f, 0.3f, 0.1f);
	image->Draw(_body->position, 0.0f, _rotation);
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

Vec2f& PhysicalTile::position(){
	return _body->position;
}

