#include "Tile.h"
#include "acPolygonShape.h"
#include "acBody.h"
#include "AImage.h"

Tile::Tile(void):_rotation(0.0f)
{
	
}


Tile::~Tile(void)
{

}

void Tile::Init(const std::string& textureName, const Recti& rect, float hw, float hh){
	acPolygonShape* boxShape = new acPolygonShape();
	boxShape->SetAsBox(hw, hh);
	_body = new acBody();
	_body->SetShape(boxShape);

	image = new AImage(textureName);
	image->setRect(rect);
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
	image->Draw(_body->position, _rotation);
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