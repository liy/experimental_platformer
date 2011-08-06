#pragma once
#include <windows.h>
#include <gl\gl.h>								// Header File For The OpenGL32 Library
#include <gl\glu.h>								// Header File For The GLu32 Library

class ASprite;
class TileMap;
class Game;
class Scene;

/**
  * Render setting will be located in this class.
  * It will trigger the Render method in the Scene class with current render setting.
  **/
class RenderModule
{
public:
	RenderModule(void);
	~RenderModule(void);

	// game pointer
	Game* _game;
	// current scene pointer
	Scene* _scene; 

	// Windows Device context handler
	HDC _hDC;

	// viewport size
	GLint _viewportRect[4];

	int Init(Game* $game, HDC& $hDC, unsigned int sw, unsigned int sh);

	// HDC: Handler for Device context
	int Render();

	// When screen resized, this function will be triggered
	void Resize(unsigned int sw, unsigned int sh);

	// Use perspective projection
	void UsePerspective();
	// Use orthogonal projection
	void UseOrthogonal();

protected:
	// screen width
	unsigned int _screenWidth;
	// screen height
	unsigned int _screenHeight;
};

