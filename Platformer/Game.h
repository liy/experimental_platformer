#pragma once
#include <windows.h>
#include <gl\gl.h>								// Header File For The OpenGL32 Library
#include <gl\glu.h>								// Header File For The GLu32 Library
#include <vector>

class GameInputHandler;
class Scene;
class RenderModule;

class Game
{
public:
	// Every second, how many times of input processsing will happen
	static const int INPUT_PER_SEC = 30;
	// Each input can only afforded time, in mini seconds.
	static const int MS_PER_INPUT = 1000/INPUT_PER_SEC;

	// Every second, how many times of update(game logic update, physic engine and graphics update) should occur
	static const int UPDATE_PER_SEC = 60;
	// The time each update can only cost, in mini seconds.
	static const int MS_PER_UPDATE = 1000/UPDATE_PER_SEC;

	// only 5 render call can ommited, if the update or input process is running to slow. After 5 times, the input or update loop will break in order to give renderer a chance to render the screen.
	static const int MAX_FRAMES_SKIP = 5;

	Game(void);
	~Game(void);

	bool quit;

	GameInputHandler* _gameInputHandler;

	RenderModule* _renderer;

	HDC _hDC;

	void Init(HDC hDC, HWND hWnd, int screenWidth, int screenHeight);

	// The main game loop
	MSG MainGameLoop();

	// Start main game loop
	MSG Start();

	LRESULT CALLBACK MsgRouter(HWND	hWnd, UINT	uMsg, WPARAM wParam, LPARAM lParam);

	void Resize(int $w, int $h);

	RenderModule& getRenderer() const;

	GameInputHandler& getGameInputHandler() const;

	Scene& getCurrentScene();

	int screenWidth();
	int screenHeight();

protected:
	// MUST NOT be changed from the class. Other than MainWindow.cpp
	int _screenWidth;
	int _screenHeight;

	// whether game finish initialization or not.
	bool _initilized;

	Scene* _currentScene;
};

