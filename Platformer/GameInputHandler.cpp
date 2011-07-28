#include "GameInputHandler.h"
#include <iostream>
#include "Game.h"
#include "IMouseEventListener.h"
#include "IWinMenuEventListener.h"
#include "GamepadEventListener.h"
#include "menu_resource.h"
#include "Scene.h"

GameInputHandler::GameInputHandler(void)
{
}


GameInputHandler::~GameInputHandler(void)
{
	// Must unacquire the device object before you release it! Otherwise serious problem could accur.
	if(_gamepadObj)
		_gamepadObj->Unacquire();

	// Release any DirectInput objects.
	SAFE_RELEASE(_ffbEffect);
	SAFE_RELEASE(_gamepadObj);
	SAFE_RELEASE(_di8Obj);
}

void GameInputHandler::Init(Game* $game, HWND& hWnd){
	_game = $game;

	_hWnd = hWnd;

	// zero num of  force feedback
	_numOfFeedbackAxis = 0;

	// store mouse listener

	InitGamepad();
}

void GameInputHandler::AddMouseEventListener(IMouseEventListener* listener){
	_mouseEventListeners.push_back(listener);
}

void GameInputHandler::RemoveMouseEventListener(IMouseEventListener* listener){
	for(int i=0; i<_mouseEventListeners.size(); ++i){
		// FIXME: Are you sure this will work??
		if(_mouseEventListeners[i] == listener){
			_mouseEventListeners.erase(_mouseEventListeners.begin()+i);
		}
	}
}

void GameInputHandler::AddWinMenuEventListener(IWinMenuEventListener* listener){
	_winMenuEventListeners.push_back(listener);
}

void GameInputHandler::AddGamepadListener(GamepadEventListener* listener){
	_gamepadListeners.push_back(listener);
}

void GetOGLPos(short x, short y, GLdouble& glx, GLdouble& gly){
	GLint viewportRect[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewportRect);

	winX = (float)x;
	winY = (float)viewportRect[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject( winX, winY, winZ, modelview, projection, viewportRect, &glx, &gly, &posZ);
}

LRESULT CALLBACK GameInputHandler::MsgHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch(uMsg)
	{
		case WM_LBUTTONDOWN:
		{
			GLdouble x, y;
			GetOGLPos((short)LOWORD(lParam), (short)HIWORD(lParam), x, y);

			//std::cout << "left click: [x:" << x <<", y:"<< y <<"]\n";

			for(int i=0; i<_mouseEventListeners.size(); ++i){
				_mouseEventListeners[i]->MouseDownHandler(x, y);
			}

		}
		case WM_MOUSEMOVE:
		{
			GLdouble x, y;

			//std::cout << "move: [x:" << (short)LOWORD(lParam) <<", y:"<< (short)HIWORD(lParam) <<"]\n";

			GetOGLPos((short)LOWORD(lParam), (short)HIWORD(lParam), x, y);

			//std::cout << "after: [x:" << x <<", y:"<< y <<"]\n";
		}
		// handle the windows menu command
		case WM_COMMAND:
		{
			// Only handle the quit command here.
			switch(LOWORD(wParam))
			{
				case ID_FILE_EXIT:
				{
					_game->quit = true;
				}
			}

			for(int i=0; i<_winMenuEventListeners.size(); ++i){
				_winMenuEventListeners[i]->WinMenuEventHandler(wParam);
			}
		}

	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

// Process input. Includes, gamepad input, mouse input and keyboard input.
// This method will be called from the MainGameLoop
void GameInputHandler::ProcessInput(){
	// process gamepad input
	HandleGamepadInput();

	// TODO: testing constant force feedback
	//UpdateDeviceForce();

	// process other input
}

//  Only Handle gamepad input.
HRESULT GameInputHandler::HandleGamepadInput()
{
	HRESULT hr;
	DIJOYSTATE2 gp;

	if(NULL == _gamepadObj)
		return S_OK;

	// Poll the device to read the current state
	if(FAILED(hr = _gamepadObj->Poll()))
	{
		// DInput is telling us that the input stream has been
		// interrupted. We aren't tracking any state between polls, so
		// we don't have any special reset that needs to be done. We
		// just re-acquire and try again.
		hr = _gamepadObj->Acquire();
		while(hr == DIERR_INPUTLOST)
			hr = _gamepadObj->Acquire();

		// hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
		// may occur when the app is minimized or in the process of 
		// switching, so just try again later 
		return S_OK;
	}
	
	
	// Get the input's device state
	if(FAILED(hr = _gamepadObj->GetDeviceState(sizeof(DIJOYSTATE2), &gp))){
		std::cout << "Get device state error\n";
		return hr;
	}
	
	// left tick control, for movement
	for(int i=0; i<_gamepadListeners.size(); ++i){
		_gamepadListeners[i]->Move(gp.lX/100.0f, gp.lY/100.0f);
	}

	// right tick control, for panning camera, debuging etc.
	for(int i=0; i<_gamepadListeners.size(); ++i){
		_gamepadListeners[i]->UpdateCamera(gp.lZ/100.0f, gp.lRz/100.0f);
	}

	if(gp.rgbButtons[0]&0x80){
		//std::cout << "jump\n";
		for(int i=0; i<_gamepadListeners.size(); ++i){
			_gamepadListeners[i]->Jump();
		}
	}
	else{
		for(int i=0; i<_gamepadListeners.size(); ++i){
			_gamepadListeners[i]->JumpRelease();
		}
	}

	// another button
	if(gp.rgbButtons[1]&0x80){
		std::cout << "lock pressed \n";
		for(int i=0; i<_gamepadListeners.size(); ++i){
			_gamepadListeners[i]->LockOn();
		}
	}

	return S_OK;
}

BOOL CALLBACK GameInputHandler::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* context)
{
	HRESULT hr;
	// create the device object, gamepad instance
	if(FAILED(hr = _di8Obj->CreateDevice(pdidInstance->guidInstance, &_gamepadObj, NULL)))
		return DIENUM_STOP;
	return DIENUM_CONTINUE;
}

BOOL CALLBACK enumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* context){
	if (context != NULL)
		return ((GameInputHandler*)context)->EnumJoysticksCallback(pdidInstance, context);
	else
		return DIENUM_STOP;
}

BOOL CALLBACK GameInputHandler::EnumObjectCallback(const DIDEVICEOBJECTINSTANCE* objInstance, VOID* pContext){
	// if the object is a acxis control
	if(objInstance->dwType & DIDFT_AXIS){
		// Set the range
		DIPROPRANGE propRange; 
		propRange.diph.dwSize       = sizeof(DIPROPRANGE); 
		propRange.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
		propRange.diph.dwHow        = DIPH_BYID; 
		propRange.diph.dwObj        = objInstance->dwType;
		propRange.lMin              = -100; 
		propRange.lMax              = 100;
		if (FAILED(_gamepadObj->SetProperty(DIPROP_RANGE, &propRange.diph))) {
			std::cout<<"set range fail\n";
			return DIENUM_STOP;
		}

		// set dead zone
		// first is x axis of the left stick
		DIPROPDWORD deadZone;
		deadZone.diph.dwSize = sizeof(deadZone);
		deadZone.diph.dwHeaderSize  = sizeof(deadZone.diph);
		deadZone.diph.dwObj = DIJOFS_X;
		deadZone.diph.dwHow = DIPH_BYOFFSET;
		// The dead zone is always in terms of 0-10000 or hundreds of a percent. If you want a dead zone of 50%, use 5000, for 10% use 1000, and so forth.
		deadZone.dwData = 5000;
		if (FAILED(_gamepadObj->SetProperty(DIPROP_DEADZONE, &deadZone.diph))) {
			std::cout<<"set left deazone x fail\n";
			return DIENUM_STOP;
		}
		// y axis
		deadZone.diph.dwSize = sizeof(deadZone);
		deadZone.diph.dwHeaderSize  = sizeof(deadZone.diph);
		deadZone.diph.dwObj = DIJOFS_Y;
		deadZone.diph.dwHow = DIPH_BYOFFSET;
		// The dead zone is always in terms of 0-10000 or hundreds of a percent. If you want a dead zone of 50%, use 5000, for 10% use 1000, and so forth.
		deadZone.dwData = 5000;
		if (FAILED(_gamepadObj->SetProperty(DIPROP_DEADZONE, &deadZone.diph))) {
			std::cout<<"set left deazone y fail\n";
			return DIENUM_STOP;
		}

		// right stick x axis
		deadZone.diph.dwSize = sizeof(deadZone);
		deadZone.diph.dwHeaderSize  = sizeof(deadZone.diph);
		deadZone.diph.dwObj = DIJOFS_Z;
		deadZone.diph.dwHow = DIPH_BYOFFSET;
		// The dead zone is always in terms of 0-10000 or hundreds of a percent. If you want a dead zone of 50%, use 5000, for 10% use 1000, and so forth.
		deadZone.dwData = 5000;
		if (FAILED(_gamepadObj->SetProperty(DIPROP_DEADZONE, &deadZone.diph))) {
			std::cout<<"set right deazone y fail\n";
			return DIENUM_STOP;
		}
		// y axis
		deadZone.diph.dwSize = sizeof(deadZone);
		deadZone.diph.dwHeaderSize  = sizeof(deadZone.diph);
		deadZone.diph.dwObj = DIJOFS_RZ;
		deadZone.diph.dwHow = DIPH_BYOFFSET;
		// The dead zone is always in terms of 0-10000 or hundreds of a percent. If you want a dead zone of 50%, use 5000, for 10% use 1000, and so forth.
		deadZone.dwData = 5000;
		if (FAILED(_gamepadObj->SetProperty(DIPROP_DEADZONE, &deadZone.diph))) {
			std::cout<<"set right deazone y fail\n";
			return DIENUM_STOP;
		}
	}

	if((objInstance->dwFlags & DIDOI_FFACTUATOR) != 0){
		_numOfFeedbackAxis++;
		std::cout << "num of force feedback axis: " << _numOfFeedbackAxis << "\n";
	}

	//std::cout << "name: " << objInstance->tszName << "\n";

	return DIENUM_CONTINUE;
}

BOOL CALLBACK enumObjectCallback(const DIDEVICEOBJECTINSTANCE* objInstance, VOID* context){
	if(context != NULL) {
		return ((GameInputHandler*)context)->EnumObjectCallback(objInstance, context);
	} else {
		return DIENUM_STOP;
	}
}

HRESULT GameInputHandler::InitDeviceForces(){
	HRESULT hr;
	DIPROPDWORD dipdw;
	// WTF?
	// Since we will be playing force feedback effects, we should disable the
	// auto-centering spring.
	dipdw.diph.dwSize = sizeof( DIPROPDWORD );
	dipdw.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = FALSE;
	if(FAILED(hr = _gamepadObj->SetProperty(DIPROP_AUTOCENTER, &dipdw.diph)))
		return hr;

	if(_numOfFeedbackAxis > 2)
		_numOfFeedbackAxis = 2;

	DWORD rgdwAxes[2] = {DIJOFS_X, DIJOFS_Y};
	// There is no direction here
	LONG rglDirection[2] = {10000, 10000};
	// constant force. having a lMagnitude member set to max 
	DICONSTANTFORCE cf = {10000};

	// setting up the force feedback parameters
	DIEFFECT ef;
	// Initialize to 0 to prevent unexpect values in the memory
	ZeroMemory(&ef, sizeof(DIEFFECT));
	ef.dwSize = sizeof(DIEFFECT);							// Size of the effect
	ef.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;		// DIEFF_CARTESIAN ensures rglDirection values are interpreted as cartesian coordinates. DIEFF_OBJECTOFFSETS saying the values of dwTriggerButton and rgdwAxes are data format offsets. 
	ef.dwDuration = (DWORD)(2 * DI_SECONDS);// The total duration of the effect, in microseconds. If this value is INFINITE, the effect has infinite duration.
	ef.dwSamplePeriod = 0;									// The period at which the device should play back the effect, in microseconds. A value of 0 indicates that the default playback sample rate should be used. 
	ef.dwGain = DI_FFNOMINALMAX;							// scaling factor applied to the magnitude of the effect
	ef.dwTriggerButton = DIEB_NOTRIGGER;					// No trigger button is associated with the effect
	ef.dwTriggerRepeatInterval = 0;							// In microseconds. Setting this value to INFINITE suppresses repetition. 
	ef.cAxes = _numOfFeedbackAxis;							// The number of axes involved in the effect
	ef.rgdwAxes = rgdwAxes;									// Pointer to a DWORD array (of cAxes elements) containing identifiers or offsets identifying the axes to which the effect is to be applied
	ef.rglDirection = rglDirection;							// There is no force direction 
	ef.lpEnvelope = NULL;										// WTF?
	ef.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);		// Size of the type-specific parameters
	ef.lpvTypeSpecificParams = &cf;							// Pointer to type-specific parameters, here is the DICONSTANTFORCE structure
	ef.dwStartDelay = 0;									// No delay
	
	// create effect
	if(FAILED(hr = _gamepadObj->CreateEffect(GUID_ConstantForce, &ef, &_ffbEffect, NULL)))
		return hr;

	std::cout << "init force feedback complete\n";
	
	return DIENUM_CONTINUE;
}

HRESULT GameInputHandler::UpdateDeviceForce(){
	// Modifying an effect is basically the same as creating a new one, except
	// you need only specify the parameters you are modifying
	LONG rglDirection[2] = { 0, 0 };

	DICONSTANTFORCE cf;


	// If only one force feedback axis, then apply only one direction and 
	// keep the direction at zero
	cf.lMagnitude = 20000;
	rglDirection[0] = 0;


	DIEFFECT eff;
	ZeroMemory( &eff, sizeof( eff ) );
	eff.dwSize = sizeof( DIEFFECT );
	eff.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	eff.cAxes = _numOfFeedbackAxis;
	eff.rglDirection = rglDirection;
	eff.lpEnvelope = 0;
	eff.cbTypeSpecificParams = sizeof( DICONSTANTFORCE );
	eff.lpvTypeSpecificParams = &cf;
	eff.dwStartDelay = 0;

	// Now set the new parameters and start the effect immediately.
	return _ffbEffect->SetParameters(&eff, DIEP_DIRECTION | DIEP_TYPESPECIFICPARAMS | DIEP_START);
}

HRESULT GameInputHandler::InitGamepad(){
	HRESULT hr;

	// create directinput8  object
	if(FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&_di8Obj, NULL)))
		return hr;

	// enumerate devices using the directInput8 object
	if(FAILED(hr = _di8Obj->EnumDevices(DI8DEVCLASS_GAMECTRL, enumJoysticksCallback, this, DIEDFL_ATTACHEDONLY | DIEDFL_FORCEFEEDBACK)))
		return hr;

	// Set the data format to "simple joystick" - a predefined data format 
	//
	// A data format specifies which controls on a device we are interested in,
	// and how they should be reported. This tells DInput that we will be
	// passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
	if(FAILED(hr = _gamepadObj->SetDataFormat(&c_dfDIJoystick2))) {
		std::cout << "dataformat fail\n";
		return hr;
	}

	// Setting the gamepad cooperative level with the application. Since we want to support force feedback game, it require exclusive access. Once the application acquired the device no other application can request exlusive access to it.
	// This is fine because I do not think you want to open another application at the same time which will also use the gamepad.(You better close the other applications)
	// The reason for setting it DISCL_BACKGROUND, make the application can handle the input while it is not  running in foreground. Easy to debug.
	if (FAILED(hr = _gamepadObj->SetCooperativeLevel(_hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND))) {
		std::cout << "SetCooperativeLevel fail\n";
		return hr;
	}

	// Determine how many axis the joystick has (so we don't error out setting
	// properties for unavailable axis)
	DIDEVCAPS capabilities;
	capabilities.dwSize = sizeof(DIDEVCAPS);
	if(FAILED(hr = _gamepadObj->GetCapabilities(&capabilities))) {
		std::cout << "GetCapabilities fail\n";
		return hr;
	}

	// Start enumerate all the buttons on that device
	if(FAILED(hr = _gamepadObj->EnumObjects(enumObjectCallback, this, DIDFT_ALL))){
		std::cout << "EnumObjects fail\n";
		return hr;
	}

	// After sucessfully create the gamepad, now program can set the force feedback
	if(FAILED(hr = InitDeviceForces())){
		std::cout << "Force feedback setup fail\n";
		return hr;
	}

	return hr;
}