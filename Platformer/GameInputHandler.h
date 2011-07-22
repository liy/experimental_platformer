#pragma once
#include <dinput.h>
#include <dinputd.h>
#include <stdio.h>
#include <strsafe.h>
#include <vector>

// For releasing and deleting DirectInput related object
#define SAFE_DELETE(p)  {if(p) {delete (p);     (p)=NULL;}}
#define SAFE_RELEASE(p) {if(p) {(p)->Release(); (p)=NULL;}}

class Game;
class Player;
class IMouseEventListener;
class IWinMenuEventListener;
class GamepadEventListener;

class GameInputHandler
{
public:
	GameInputHandler(void);
	~GameInputHandler(void);

	void Init(HWND hWnd, Game* game);

	void ProcessInput();

	// Handling user mouse click
	LRESULT CALLBACK MsgHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* deviceInstance, VOID* context);

	BOOL CALLBACK EnumObjectCallback(const DIDEVICEOBJECTINSTANCE* objInstance, VOID* context);

	HRESULT UpdateDeviceForce();

	void AddMouseEventListener(IMouseEventListener* listener);
	void RemoveMouseEventListener(IMouseEventListener* listener);

	void AddWinMenuEventListener(IWinMenuEventListener* listener);

	void AddGamepadListener(GamepadEventListener* listener);

protected:
	HWND _hWnd;

	// directinput8 object
	LPDIRECTINPUT8 _di8Obj;

	// directinput8 device object
	LPDIRECTINPUTDEVICE8 _gamepadObj;

	DWORD _numOfFeedbackAxis;

	// Force feedback effect
	LPDIRECTINPUTEFFECT _ffbEffect;

	Game* _game;

	//AAnimation* _player;
	Player* _player;

	// FIXME: change into Map, or Set.
	// The mouse listeners
	std::vector<IMouseEventListener*> _mouseEventListeners;

	std::vector<IWinMenuEventListener*> _winMenuEventListeners;

	std::vector<GamepadEventListener*> _gamepadListeners;

	HRESULT InitGamepad();

	HRESULT InitDeviceForces();

	HRESULT HandleGamepadInput();

	typedef struct DIR_INPUT{
		int dx;
		int dy;
	};

	typedef struct ACTION_INPUT{
		// jump
		// grab
		// gesture
	};

	typedef struct OTHER_INPUT{
		// menu
		// inventory
	};
};

