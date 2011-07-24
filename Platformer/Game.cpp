#include <iostream>

#include "Game.h"
#include "GameInputHandler.h"
#include "ATextureManager.h"
#include "Scene.h"
#include "RenderModule.h"

Game::Game(void): quit(false), _initilized(false)
{

}


Game::~Game(void)
{
	delete _gameInputHandler;
	_gameInputHandler = NULL;

	delete _renderer;
	_renderer = NULL;
}

void Game::Init(HDC hDC, HWND hWnd, int screenWidth, int screenHeight){
	_hDC = hDC;

	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	// You must initialize the TextureManager first before use.
	ATextureManager::GetInstance()->Init();

	// create input handler
	_gameInputHandler = new GameInputHandler();
	_gameInputHandler->Init(hWnd, this);

	// create simple a game scene
	_currentScene = new Scene(*this);
	_currentScene->Init(_screenWidth, _screenHeight);

	// create renderer
	_renderer = new RenderModule(_hDC);
	_renderer->Init(this);
	// setup projection
	_renderer->SetupProjection(_screenWidth, _screenHeight);

	
	_gameInputHandler->AddMouseEventListener(_currentScene);

	_initilized = true;
}

MSG Game::Start(){
	// start the mainloop....
	MSG msg = MainGameLoop();

	return msg;
}

// simply route the window message to the InputHandler instance.
LRESULT CALLBACK Game::MsgRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	return _gameInputHandler->MsgHandler(hWnd, uMsg, wParam, lParam);
}

void Game::Resize(int $w, int $h){
	if(_initilized){
		_screenWidth = $w;
		_screenHeight = $h;

		_renderer->Resize(_screenWidth, _screenHeight);
	}
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
			// If the time for updating is not arrived yet then do the loop will not be executed, or if the max number of frames rendering can skippied is exceeded
			while(GetTickCount() >= inputTime && loops < MAX_FRAMES_SKIP){
				// It is just the time for or the time is running behind for input process, we should do the input process again in order to keep up of the time. Unless the max number of frams can be skipped is exceeded.
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

				updateTime += MS_PER_UPDATE;
				++loops;
			}
			
			// processing rendering
			// render the whole thing, as much as possible. Do not care about dupldate frames rendering
			_renderer->Render();

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

int Game::screenWidth(){
	return _screenWidth;
}

int Game::screenHeight(){
	return _screenHeight;
}