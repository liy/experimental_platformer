#pragma once
#include "IMouseEventListener.h"

class Game;
class Actor;
class Tile;

static const int NUM_TILES = 100;

class Scene : public IMouseEventListener
{
public:

	Scene(Game& $game);
	~Scene(void);

	Tile* tiles;

	void Init();

	// update the game logic, physic engine, together with animation, graphics update
	// provide the a delta time.
	void Update(unsigned short deltaTime);

	// After user input handled(Game class delegate the input to GameInputHandler class), followed by Update method.
	// All the game object rendering should appear here.
	void Render();

	// override mouse down handler  function in MouseListener
	void MouseDownHandler(short x, short y);
	void MouseMoveHandler(short x, short y);
protected:
	// TODO: just for testing pointer and reference, if we want to have a class scoped Game reference, we have to initialize the reference in the constructor parameter list.
	// Because a reference is invalid with out a referant.
	Game& _game;
	
	Actor* actor;
};

