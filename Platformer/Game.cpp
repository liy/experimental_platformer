#include <iostream>
#include "game_settings.h"
#include "graphics_engine.h"
#include "Game.h"
#include "GameInputHandler.h"
#include "ATextureCache.h"
#include "Scene.h"
#include "RenderModule.h"
#include "Camera.h"
#include "GameEditor.h"



Game::Game(void): quit(false)
{

}


Game::~Game(void)
{
	delete _gameInputHandler;
	_gameInputHandler = NULL;

	delete _renderer;
	_renderer = NULL;


	delete editor;

}

void Game::Init(HDC hDC, HWND hWnd, int screenWidth, int screenHeight){
	_hDC = hDC;
	_hWnd = hWnd;

	// initialize the graphics engine
	InitGraphicsEngine();

	// initialize camera
	camera = new Camera(this);
	camera->Init(screenWidth, screenHeight, 0.5, 0.5);

	// You must initialize the TextureManager first before use.
	ATextureCache::GetInstance()->Init();

	// create input handler
	_gameInputHandler = new GameInputHandler();
	_gameInputHandler->Init(this, hWnd);

	// create simple a game scene
	_currentScene = new Scene();
	_currentScene->Init(this);
	_gameInputHandler->AddMouseEventListener(_currentScene);

	// create renderer, by default using orthogonal projection
	_renderer = new RenderModule();
	_renderer->Init(this, _hDC, screenWidth, screenHeight);


	editor = new GameEditor(this);
}

MSG Game::Start(){
	// start the main loop....
	MSG msg = MainGameLoop();

	return msg;
}

// simply route the window message to the InputHandler instance.
LRESULT CALLBACK Game::MsgRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	editor->MsgHandler(hWnd, uMsg, wParam, lParam);

	return _gameInputHandler->MsgHandler(hWnd, uMsg, wParam, lParam);
}

void Game::Resize(int $w, int $h){
	_renderer->Resize($w, $h);
}


long _frameCount;
DWORD _previousTime;
int _fps;
void DisplayFPS(HWND hWnd){
	_frameCount++;
	//  Get the number of milliseconds since glutInit called
	//  (or first call to glutGet(GLUT ELAPSED TIME)).
	DWORD currentTime = GetTickCount();
	//  Calculate time passed
	int timeInterval = currentTime - _previousTime;
	// every half second do a FPS update.
	if(timeInterval > 500)
	{
		//  calculate the number of frames per second
		_fps = _frameCount / (timeInterval / 1000.0f);

		//  Set time
		_previousTime = currentTime;
		//  Reset frame count
		_frameCount = 0;
	}

	// convert to string
	char str[10];
	sprintf(str, "FPS: %d", _fps);
	SetWindowText(hWnd, str);
}

MSG Game::MainGameLoop(){
	MSG msg;
	
	// The last timestamp when update is called
	DWORD lastUpdateTime = GetTickCount();
	// Just for calculate the delta time between different update call.
	unsigned short deltaTime = 0;
	// The time when the input process should be finished
	DWORD inputTime = GetTickCount();
	// The time when the update process should be finished
	DWORD updateTime = GetTickCount();
	// current number of loops in input processing or update processing
	int loops = 0;

	// keep looping until we quite the window(done will set to true)
	while(!quit){
		// The first thing we have to do is check to see if any window messages are waiting. By using PeekMessage() we can check for messages without halting our program. 
		// A lot of programs use GetMessage(). It works fine, but with GetMessage() your program doesn't do anything until it receives a paint message or some other window message.
		// In other word.................. PeekMessage does not block, GetMessage will!!!!!
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))			// Is There A Message Waiting?
		{
			// received a quit message. So set done to true, get out of the loop
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				// FIXME: do we still need to call TranslateMessage and DispatchMessage, if we are exiting the program???
				quit = true;				// exit the program.
			}
			else							// If Not, Deal With Window Messages
			{
				// If it is normal message, first we have to translate the message. Then we send out the message to the desired window. How the message dispatch to correct window is auto handled by the system.
				// 
				// If the message isn't a quit message we translate the message then dispatch the message so that WndProc() or Windows can deal with it. 	
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		// If there is no message waiting to be processed, try to draw scene and handle our customized key press action.
		else					// If There Are No Messages
		{
			// processing input
			loops = 0;
			// If the time for updating is not arrived yet then do the loop will not be executed, or if the max number of frames rendering can skipped is exceeded
			while(GetTickCount() >= inputTime && loops < MAX_FRAMES_SKIP){
				// It is just the time for or the time is running behind for input process, we should do the input process again in order to keep up of the time. Unless the max number of frames can be skipped is exceeded.
				// process input, include keyboard, mouse and gamepad input...
				_gameInputHandler->ProcessInput();

				inputTime += MS_PER_INPUT;
				++loops;
			}
			
			// processing update
			loops = 0;
			while(GetTickCount() > updateTime && loops < MAX_FRAMES_SKIP){
				// first update the physics collision detection
				//_physicsEngine->Update();

				DWORD currentTime = GetTickCount();
				// calculate the delta mini seconds between different update call.
				deltaTime = currentTime - lastUpdateTime;
				// ready for next delta calculation, the current time become to the last update time.
				lastUpdateTime = currentTime;

				//std::cout << "deltaTime: " << deltaTime <<"\n";
				// then update game state and animation, etc.
				_currentScene->Update(deltaTime);

				editor->Update();

				updateTime += MS_PER_UPDATE;
				++loops;
			}
			
			// processing rendering
			// render the whole thing, as much as possible. Do not care about duplicate frames rendering
			_renderer->Render();

			DisplayFPS(_hWnd);

		} // no message else end.
	} // while end

	return msg;
}

RenderModule& Game::getRenderer() const{
	return *_renderer;
}

GameInputHandler& Game::getGameInputHandler() const{
	return *_gameInputHandler;
}

Scene& Game::getCurrentScene(){
	return *_currentScene;
}