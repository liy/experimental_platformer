#include "Scene.h"
#include <iostream>
#include "Game.h"
#include "GameInputHandler.h"
#include "game_settings.h"
#include "acMath.h"
#include "Actor.h"
#include "acBody.h"
#include "acPolygonShape.h"
#include "AImage.h"
#include "AAnimation.h"
#include "PhysicalTile.h"
#include "Camera.h"
#include "GraphicalTile.h"

Scene::Scene(void)
{

}


Scene::~Scene(void)
{
	
}

GraphicalTile* graphicalTile;

void Scene::Init(Game* $game){
	 _game = $game;

	// create actor's rigid body
	acBody* body = new acBody();
	acPolygonShape shape;
	shape.SetAsBox(7.0f, 14.0f);
	body->SetShape(&shape);

	// create the animation
	std::vector<AFrame*> frames;
	int trackX = 0;
	for(int i=0; i<3; ++i){
		AFrame* frame = new AFrame("texture.png", 7);
		frame->setRect(trackX, 0, 17, 31);
		trackX+=18;
		frames.push_back(frame);
	}
	AAnimation* ani = new AAnimation(frames);
	ani->pingpong = true;
	ani->Play();

	//AImage* image = new AImage("texture.png");
	//image->setRect(0, 0, 17, 30);
	// created a actor
	actor = new Actor(this, body);
	actor->animation_ptr = ani;
	actor->SetPosition(Vec2f(400.0f, 200.0f));

	_game->getGameInputHandler().AddGamepadListener(actor);
	_game->getGameInputHandler().AddGamepadListener(this);


	// initialize tiles
	float tx = 96.0f;
	float ty = 98.0f;

	tiles = new PhysicalTile[NUM_TILES];

	tiles[0] = PhysicalTile();
	tiles[0].Init("tileset.png", Recti(0, 0, 32, 32), 14.0f, 14.0f);
	tiles[0].position().Set(0, 0);

	tx += 64.0f;

	tiles[1] = PhysicalTile();
	tiles[1].Init("tileset.png", Recti(0, 0, 32, 32), 14.0f, 14.0f);
	tiles[1].position().Set(tx+=32.0f, ty);

	tiles[2] = PhysicalTile();
	tiles[2].Init("tileset.png", Recti(0, 0, 32, 32), 14.0f, 14.0f);
	tiles[2].position().Set(tx+=32.0f, ty);

	tiles[3] = PhysicalTile();
	tiles[3].Init("tileset.png", Recti(0, 0, 32, 32), 14.0f, 14.0f);
	tiles[3].position().Set(tx+=32.0f, ty);

	tiles[4] = PhysicalTile();
	tiles[4].Init("tileset.png", Recti(0, 0, 32, 32), 14.0f, 14.0f);
	tiles[4].position().Set(tx+=32.0f, ty);

	tiles[5] = PhysicalTile();
	tiles[5].Init("tileset.png", Recti(0, 0, 32, 32), 14.0f, 14.0f);
	tiles[5].position().Set(tx, ty-=32.0f);

	tiles[6] = PhysicalTile();
	tiles[6].Init("tileset.png", Recti(0, 0, 32, 32), 14.0f, 14.0f);
	tiles[6].position().Set(tx, ty-=32.0f);

	for(int i=7; i<NUM_TILES; ++i){
		tiles[i] = PhysicalTile();
		tiles[i].Init("tileset.png", Recti(0, 0, 32, 32), 14.0f, 14.0f);
		tiles[i].position().Set(int((unifRand()*2024.0f)/32.0f)*32.0f, int((unifRand()*768.0f)/32.0f)*32.0f);
	}

	// lock the camera to the actor
	_game->camera->Follows(*actor);

	graphicalTile = new GraphicalTile(new AAnimation(frames));
	AAnimation* a = dynamic_cast<AAnimation*>(graphicalTile->graphics());
	if(a != NULL){
		a->Play();
	}
	graphicalTile->position().Set(300.0f, 100.0f);
}

void Scene::Update(unsigned short delta){
	actor->Update(delta);

	for(int i=0; i<NUM_TILES; ++i){
		tiles[i].Update(delta);
	}

	graphicalTile->Update(delta);

	_game->camera->Update(delta);

}


// After user input handled(Game class delegate the input to GameInputHandler class), followed by Update method.
// All the game object rendering should appear here.
void Scene::Render(){
	_game->camera->Setup();

	//std::cout << "render called\n";
	actor->Draw();

	for(int i=0; i<NUM_TILES; ++i){
		tiles[i].Draw();
	}

	glColor3f(0.0f, 0.0f, 1.0f);
	glPointSize(10.0f);
	glBegin(GL_POINTS);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();

	graphicalTile->Draw();
}

void Scene::MouseDownHandler(short x, short y){
	std::cout << "Scene left click: [x:" << x <<", y:"<< y <<"]\n";
	Vec2f c = Vec2f(x, y);
	actor->SetPosition(c);
}

void Scene::MouseMoveHandler(short x, short y){
	
}

void Scene::Move(float xRatio, float yRatio){

}

void Scene::UpdateCamera(float xRatio, float yRatio){
	if(yRatio < 0.0f){
		Vec2f size = _game->camera->GetViewportSize();
		size -= size*0.01;
		_game->camera->ResizeViewport(size.x, size.y);
	}
	else if(yRatio > 0.0f){
		Vec2f size = _game->camera->GetViewportSize();
		size += size*0.01;
		_game->camera->ResizeViewport(size.x, size.y);
	}

	if(xRatio < 0.0f){
		_game->camera->rotation += 0.4f;
	}
	else if(xRatio > 0.0f){
		_game->camera->rotation -= 0.4f;
	}

}

void Scene::Jump(){

}
void Scene::JumpRelease(){

}
void Scene::Stop(){

}

void Scene::LockOn(){
	if(_game->camera->GetLockedTarget() == NULL){
		_game->camera->Follows(*actor);
	}
	else{
		_game->camera->Unlock();
	}
}