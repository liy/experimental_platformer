#include "Tile.h"
#include "acPolygonShape.h"
#include "acBody.h"

Tile::Tile(void)
{
	acPolygonShape* boxShape = new acPolygonShape();
	boxShape->SetAsBox(16.0f, 16.0f);
	_body = new acBody();
	_body->SetShape(boxShape);
}


Tile::~Tile(void)
{
}

void Tile::SetBody(acBody* body){
	_body = body;
}

acBody* Tile::GetBody(){
	return _body;
}

void Tile::Update(){
	_body->Synchronize();
}
	
void Tile::Draw(){
	_body->DrawAABB(0.0f, 0.3f, 0.1f);
}

float Tile::rotation(){
	//return _rotation;
	return _body->rotation;
}

void Tile::setRotation(float $rotation){
	_body->rotation = $rotation;
}

Vec2f& Tile::position(){
	return _body->position;
}