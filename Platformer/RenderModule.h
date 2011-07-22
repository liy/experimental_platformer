#pragma once
#include <windows.h>
#include <gl\gl.h>								// Header File For The OpenGL32 Library
#include <gl\glu.h>								// Header File For The GLu32 Library

class AImage;
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
	RenderModule(HDC $hDC);
	~RenderModule(void);

	// game pointer
	Game* _game;
	// current scene pointer
	Scene* _scene; 

	// Windows Device context handler
	HDC _hDC;

	// viewport size
	GLint _viewportRect[4];

	int Init(Game* $game);

	void BuildFont(LPCSTR fontName);

	// HDC: Handler for Device context
	int Render();

	void Resize(int $w, int $h);

protected:
	long _frameCount;
	GLint _currentTime;
	GLint _previousTime;
	int _fps;
	GLuint _base;

	void DrawFPS();
};

