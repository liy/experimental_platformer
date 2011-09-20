// *****************************************************
//
// Modified from NEHE:
// http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=01
//
// *****************************************************

#include <windows.h>							// Header File For Windows
#include <iostream>
#include <gl\gl.h>								// Header File For The OpenGL32 Library
#include <gl\glu.h>								// Header File For The GLu32 Library
#include <ctime>

#include "Game.h"
#include "menu_resource.h"

// handle for rendering context. Setup Rendering context which links to the Device context.
HGLRC           hRC=NULL;							// Permanent Rendering Context
// handle for device context. Setup Device context which connects the windows to GDI (Graphics Device Interface, manage how to display the graphics on monitor etc.)
HDC             hDC=NULL;							// Private GDI Device Context
// handle for the windowe
HWND            hWnd=NULL;							// Holds Our Window Handle
// The window instance handle
HINSTANCE       hInstance;							// Holds The Instance Of The Application

// For some reason, nehe declare CreateGLWindow() function  before WindProc() function. So he declare the function skeloton here...
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);				// Declaration For WndProc

// window class name
const char WND_CLASS_NAME[] = "Platformer";
char WINDOW_NAME[] = "正义的城管";

Game* _game;

static const unsigned int SCREEN_WIDTH = 1024;
static const unsigned int SCREEN_HEIGHT = 788;




// window quit handling code
GLvoid KillGLWindow(GLvoid)							// Properly Kill The Window
{
	// start render context clean up
	// first to check whether we have the reder contex
	if (hRC)								// Do We Have A Rendering Context?MsgRouter
	{
		// Checking whether we can free both DC and RC
		//
		// If we have a Rendering Context, the code below will check to see if we are able to release it (detach the hRC from the hDC). Notice the way I'm checking for errors. 
		// I'm basically telling our program to try freeing it (with wglMakeCurrent(NULL,NULL), then I check to see if freeing it was successful or not. Nicely combining a few lines of code into one line. 
		if(!wglMakeCurrent(NULL, NULL)){
			MessageBox(NULL,"Release Of \"Device context\" And \"Render Context\" Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		// start Delete the render context.
		if(!wglDeleteContext(hRC)){ // are we able to delete render context?
			MessageBox(NULL, "Release \"Render Context\" failled.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		// Null the render context.
		hRC = NULL;
	}

	// If hDC exist and we try to realse hDC failed
	if(hDC && !ReleaseDC(hWnd, hDC)){
		MessageBox(NULL,"Release \"Device Context\" Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC=NULL;
	}

	// try to destroy the window
	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;							// Set hWnd To NULL
	}

	// unregister window class. So next time we don't get the "windows class already registered" error.
	if (!UnregisterClass(WND_CLASS_NAME, hInstance))				// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;							// Set hInstance To NULL
	}
}

// Create OpenGL window
BOOL CreateGLWindow(char* title, int width, int height, int bits)
{
	// WTF??!! ARGB? RGBA? RGB?
	// When we ask Windows to find us a pixel format that matches the one we want, the number of the mode that Windows ends up finding for us will be stored in the variable PixelFormat. 
	GLuint PixelFormat;						// Holds The Results After Searching For A Match

	

	// dwExStyle and dwStyle will store the Extended and normal Window Style Information. 
	// I use variables to store the styles so that I can change the styles depending on what type of window I need to create (A popup window for fullscreen or a window with a border for windowed mode) 	
	DWORD dwExStyle;						// Window Extended Style
	DWORD dwStyle;						// Window Style

	// Normally if we create a 640x480 window, the borders of the window take up some of our resolution. Call AdjustWindowRectEx() function later to ensure the actual content to be 640*480 and the whole window will be larger because of the border.
	RECT WindowRect;							
	WindowRect.left		= (long)300;
	WindowRect.right	= WindowRect.left + (long)width;		
	WindowRect.top		= (long)300;							
	WindowRect.bottom	= WindowRect.top + (long)height;

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window

	// Some one said this class structure is out of date: http://www.falloutsoftware.com/tutorials/win/win1.htm
	// Window Classs structure, contains the information about our window. Every window belongs to a Window Class, we MUST register a class for the window.
	//WNDCLASS	wc;							// Windows Class Structure
	//wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Move, And Own DC For Window
	//wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	//wc.cbClsExtra		= 0;									// No Extra Window Data
	//wc.cbWndExtra		= 0;									// No Extra Window Data
	//wc.hInstance		= hInstance;							// Set The Instance
	//wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	//wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	//wc.hbrBackground	= NULL;									// No Background Required For GL, set by OpenGL call, setClearColor() function
	//wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	//wc.lpszClassName	= WND_CLASS_NAME;						// Set The Class Name

	// Use new extended Window class structure.
	WNDCLASSEX ex;
	ex.cbSize			= sizeof(WNDCLASSEX);
	ex.style			= CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
	ex.lpfnWndProc		= (WNDPROC) WndProc;
	ex.cbClsExtra		= 0;
	ex.cbWndExtra		= 0;
	ex.hInstance		= hInstance;
	ex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	ex.hCursor			= LoadCursor(NULL, IDC_ARROW);
	ex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	ex.lpszMenuName		= NULL;
	ex.lpszClassName	= WND_CLASS_NAME;
	ex.hIconSm			= NULL;


	// register 
	if (!RegisterClassEx(&ex))						// Attempt To Register The Window Class, use extend class....
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Exit And Return FALSE
	}

	
	// load the menue from the resource
	HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));

	// If we're using a window instead of fullscreen mode, we'll add WS_EX_WINDOWEDGE to the extended style. This gives the window a more 3D look. 
	// For style we'll use WS_OVERLAPPEDWINDOW instead of WS_POPUP. WS_OVERLAPPEDWINDOW creates a window with a title bar, sizing border, window menu, and minimize / maximize buttons. 
	/*dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;*/			// Window Extended Style
	dwExStyle		= WS_EX_APPWINDOW | WS_EX_ACCEPTFILES;
	dwStyle			= WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;			// Windows Style


	// Since normally if we set window size to 640*480 the window border and menu, etc. will take up part of the size. The line of code make the actual content size to 640*480, and the whole window will be bigger because the border. Under fullscreen mode, this command has no effect. 
	//
	// The line below adjust our window depending on what style of window we are creating. The adjustment will make our window exactly the resolution we request. 
	// Normally the borders will overlap parts of our window. By using the AdjustWindowRectEx command none of our OpenGL scene will be covered up by the borders, instead, 
	// the window will be made larger to account for the pixels needed to draw the window border. In fullscreen mode, this command has no effect. 
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	

	// Create the window
	// WS_CLIPSIBLINGS and WS_CLIPCHILDREN along with the style of window we've decided to use. WS_CLIPSIBLINGS and WS_CLIPCHILDREN are both REQUIRED for OpenGL to work properly. 
	// These styles prevent other windows from drawing over or into our OpenGL Window. 
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								WND_CLASS_NAME,							// Class Name
								title,								// Window Title
								dwStyle,							// Selected Window Style
								WindowRect.left, WindowRect.top,	// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Adjusted Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Adjusted Window Height
								NULL,								// No Parent Window
								hMenu,								// Menu
								hInstance,							// Instance
								NULL)))								// Don't Pass Anything To WM_CREATE
	{
		// Not successfull, kill the window
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	static PIXELFORMATDESCRIPTOR pfd =					// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),					// Size Of This Pixel Format Descriptor
		1,												// Version Number
		PFD_DRAW_TO_WINDOW |							// Format Must Support Window
		PFD_SUPPORT_OPENGL |							// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,								// Must Support Double Buffering
		PFD_TYPE_RGBA,									// Request An RGBA Format
		bits,											// Select Our Color Depth
		0, 0, 0, 0, 0, 0,								// Color Bits Ignored
		0,												// No Alpha Buffer
		0,												// Shift Bit Ignored
		0,												// No Accumulation Buffer
		0, 0, 0, 0,										// Accumulation Bits Ignored
		16,												// 16Bit Z-Buffer (Depth Buffer)
		1,												// enable Stencil Buffer ******************************************************************************
		1,												// enable Auxiliary Buffer ******************************************************************************
		PFD_MAIN_PLANE,									// Main Drawing Layer
		0,												// Reserved
		0, 0, 0											// Layer Masks Ignored
	};

	// Attemp to get a OpenGL device context
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	// Try to find a pixel format that match the one described above.
	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))				// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	// Try to set the pixel format.
	if(!SetPixelFormat(hDC,PixelFormat,&pfd))				// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	// Try create the render context 
	if (!(hRC=wglCreateContext(hDC)))					// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	// After both render and device context created, Now we can active Render context!!
	if(!wglMakeCurrent(hDC,hRC))						// Try To Activate The Rendering Context
	{
		KillGLWindow();							// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// Return FALSE
	}

	// Note: Original specified width and height cannot be used.
	// alway get the real window rectangle, after the window is upated. 
	GetClientRect(hWnd, &WindowRect);

	// initialize the game
	_game->Init(hDC, hWnd, WindowRect.right-WindowRect.left, WindowRect.bottom-WindowRect.top);
	
	
	// show the window
	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	// What is this for?? update windows???? update what??
	UpdateWindow(hWnd);
	// bring the window forward
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	// set focus
	SetFocus(hWnd);								// Sets Keyboard Focus To The Window

	return TRUE;
}

// Each windows message may have up to two parameters, wParam and lParam. Originally wParam was 16 bit and lParam was 32 bit, but in Win32 they are both 32 bit. Not every message uses these parameters, 
// and each message uses them differently. For example the WM_CLOSE message doesn't use either, and you should ignore them both. The WM_COMMAND message uses both, wParam contains two values, 
// HIWORD(wParam) is the notification message (if applicable) and LOWORD(wParam) is the control or menu id that sent the message. lParam is the HWND (window handle) to the control which sent the message or NULL if the messages isn't from a control. 
LRESULT CALLBACK WndProc(	HWND	hWnd,					// Handle For This Window
							UINT	uMsg,					// Message For This Window
							WPARAM	wParam,					// Additional Message Information
							LPARAM	lParam)					// Additional Message Information
{
	switch (uMsg)								// Check For Windows Messages
	{
		// check if the message is a activate message. Then set the active variable
		case WM_ACTIVATE:						// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
				//active=TRUE;						// Program Is Active
				;
			else
				//active=FALSE;						// Program Is No Longer Active
				;
			return 0;							// Return To The Message Loop
		}

		case WM_SYSCOMMAND:						// Intercept System Commands
		{
			switch (wParam)						// Check System Calls
			{
				// By return 0 for both screensaver and enter powersave message we can prevent it happenning
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;							// Exit
		}

		// If uMsg is WM_CLOSE the window has been closed. We send out a quit message that the main loop will intercept. The variable done will be set to TRUE, the main loop in WinMain() will stop, and the program will close. 
		case WM_CLOSE:							// Did We Receive A Close Message?
		{
			PostQuitMessage(0);					// Send A Quit Message
			return 0;							// Jump Back
		}

		/*
		// We can find what key is hold down by reading wParam
		case WM_KEYDOWN:						// Is A Key Being Held Down?
		{
			//keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;							// Jump Back
		}

		// key up
		case WM_KEYUP:							// Has A Key Been Released?
		{
			//keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;							// Jump Back
		}
		*/

		// User are resizing the window, so call the ReSizeGLScene() function, get the width and height by using LOWORD() and HIWORD()
		// HIWORD() and LOWORD() are macros defined by windows that single out the two high bytes (High Word) of a 32 bit value (0xFFFF0000) and the low word (0x0000FFFF) respectively. 
		// In Win32 a WORD is a 16bit value, making DWORD (or Double Word) a 32bit value. 
		case WM_SIZE:							// Resize The OpenGL Window
		{
			// width is contained in lParam lower part, and height in higher part
			_game->Resize(LOWORD(lParam), HIWORD(lParam));		// LoWord=Width, HiWord=Height
			return 0;							// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	//return DefWindowProc(hWnd,uMsg,wParam,lParam);

	// Pass all the other message to the game to let game, actually left GameInputHandler to handle
	return _game->MsgRouter(hWnd, uMsg, wParam, lParam);
}

// It is like the main() function in java. Entry point of our Windows Application.
int WINAPI WinMain(	HINSTANCE	hInstance,				// Instance
					HINSTANCE	hPrevInstance,			// Previous Instance
					LPSTR		lpCmdLine,				// Command Line Parameters
					int		nCmdShow)					// Window Show State
{
	// random number seed
	srand(time(0));

	// create console output
	AllocConsole();
	freopen("conin$","r",stdin);
	freopen("conout$","w",stdout);
	freopen("conout$","w",stderr);

	// create the game
	_game = new Game();

	// FIXME: Rearrange the window creation process!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// Create Our OpenGL Window
	if (!CreateGLWindow(WINDOW_NAME, SCREEN_WIDTH, SCREEN_HEIGHT, 32))
	{
		return 0;							// Quit If Window Was Not Created
	}

	
	// start the game when the game finished kill the window
	MSG	msg = _game->Start();

	// destroy the game when it is finished... main loop stopped
	delete _game;

	// when game stopped, shut down the application and destroy the window
	KillGLWindow();
	
	return (msg.wParam);
}