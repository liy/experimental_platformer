#include "Scene.h"
#include <iostream>
#include "Game.h"
#include "GameInputHandler.h"
#include "game_settings.h"
#include "acMath.h"
#include "Actor.h"
#include "acBody.h"
#include "acPolygonShape.h"
#include "AAnimation.h"
#include "Tile.h"
#include "Camera.h"

Scene::Scene(Game& $game): _game($game)
{

}


Scene::~Scene(void)
{
	
}

void Scene::Init(int screenWidth, int screenHeight){
	// initialize camera
	camera = new Camera();
	camera->Init(screenWidth, screenHeight, 0.5, 0.3);

	// create actor's rigid body
	acBody* body = new acBody();
	acPolygonShape shape;
	shape.SetAsBox(7.0f, 14.0f);
	body->SetShape(&shape);

	// create the animation
	/*
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
	//ani->Play();
	*/

	AImage* image = new AImage("texture.png");
	image->setRect(0, 0, 17, 30);
	// created a actor
	actor = new Actor(this, body);
	actor->image_ptr = image;
	actor->SetPosition(Vec2f(400.0f, 200.0f));

	_game.getGameInputHandler().AddGamepadListener(actor);
	_game.getGameInputHandler().AddGamepadListener(this);


	// intialize tiles
	float tx = 96.0f;
	float ty = 98.0f;

	tiles = new Tile[NUM_TILES];

	tiles[0] = Tile();
	tiles[0].position().Set(tx, 65.0f);

	tx += 64.0f;

	tiles[1] = Tile();
	tiles[1].position().Set(tx+=32.0f, ty);

	tiles[2] = Tile();
	tiles[2].position().Set(tx+=32.0f, ty);

	tiles[3] = Tile();
	tiles[3].position().Set(tx+=32.0f, ty);

	tiles[4] = Tile();
	tiles[4].position().Set(tx+=32.0f, ty);

	tiles[5] = Tile();
	tiles[5].position().Set(tx, ty-=32.0f);

	tiles[6] = Tile();
	tiles[6].position().Set(tx, ty-=32.0f);

	for(int i=7; i<NUM_TILES; ++i){
		tiles[i] = Tile();
		tiles[i].position().Set(int((unifRand()*1024.0f)/32.0f)*32.0f, int((unifRand()*768.0f)/32.0f)*32.0f);
	}

	// lock the camera to the actor
	
	camera->LockOn(*actor);
}

void Scene::Update(unsigned short deltaTime){
	actor->Update(deltaTime);

	for(int i=0; i<NUM_TILES; ++i){
		tiles[i].Update();
	}

	camera->Update();
}


// After user input handled(Game class delegate the input to GameInputHandler class), followed by Update method.
// All the game object rendering should appear here.
void Scene::Render(){
	camera->Setup();

	//std::cout << "render called\n";
	actor->Draw();

	for(int i=0; i<NUM_TILES; ++i){
		tiles[i].Draw();
	}
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
		Vec2f size = camera->GetViewportSize();
		size -= size*0.01;
		camera->ResizeViewport(size.x, size.y);
	}
	else if(yRatio > 0.0f){
		Vec2f size = camera->GetViewportSize();
		size += size*0.01;
		camera->ResizeViewport(size.x, size.y);
	}

	if(xRatio < 0.0f){
		camera->rotation += 0.4f;
	}
	else if(xRatio > 0.0f){
		camera->rotation -= 0.4f;
	}
}

void Scene::Jump(){

}
void Scene::JumpRelease(){

}
void Scene::Stop(){

}

void Scene::LockOn(){
	if(camera->GetLockedTarget() == NULL){
		camera->LockOn(*actor);
	}
	else{
		camera->Unlock();
	}
}