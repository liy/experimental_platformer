#include "RenderModule.h"
#include "ATexture.h"
#include "ATextureManager.h"
#include "AImage.h"
#include "Game.h"
#include "Scene.h"

RenderModule::RenderModule(HDC $hDC): _hDC($hDC)
{

}


RenderModule::~RenderModule(void)
{
	_game = NULL;
	_scene = NULL;
}

int RenderModule::Init(Game* $game){
	std::cout << "Render init\n";

	_game = $game;
	_scene = &_game->getCurrentScene();

	// use arial font for opengl text displaying.
	BuildFont("Arial");

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	//enable transparency.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//set the background colour
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	return 0;
}

void RenderModule::BuildFont(LPCSTR fontName){
	HFONT font;
	HFONT oldfont;

	_base = glGenLists(96);
	
	font = CreateFont(-14, 0, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, fontName);

	oldfont = (HFONT)SelectObject(_hDC, font);		// Selects The Font We Want
	wglUseFontBitmaps(_hDC, 32, 96, _base);			// Builds 96 Characters Starting At Character 32
	SelectObject(_hDC, oldfont);				// Selects The Font We Want
	DeleteObject(font);					// Delete The Font
}

void RenderModule::DrawFPS(){
	_frameCount++;
	//  Get the number of milliseconds since glutInit called
	//  (or first call to glutGet(GLUT ELAPSED TIME)).
	_currentTime = glutGet(GLUT_ELAPSED_TIME);
	//  Calculate time passed
	int timeInterval = _currentTime - _previousTime;
	if(timeInterval > 500)
	{
		//  calculate the number of frames per second
		_fps = _frameCount / (timeInterval / 1000.0f);

							// Pops The Display List Bits	( NEW )

		//  Set time
		_previousTime = _currentTime;
		//  Reset frame count
		_frameCount = 0;
	}

	glPushMatrix();
	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 1.0f);

	// Position The Text On The Screen
	glRasterPos2f(10.0f, _game->screenHeight() - 24.0f);
	char fmt[10];
	itoa(_fps, fmt, 10); // conver integer to char
	char  text[10];				// Holds Our String
	va_list	ap;					// Pointer To List Of Arguments
	va_start(ap, fmt);					// Parses The String For Variables
	vsprintf(text, fmt, ap);				// And Converts Symbols To Actual Numbers
	va_end(ap);						// Results Are Stored In Text
	glPushAttrib(GL_LIST_BIT);				// Pushes The Display List Bits		( NEW )
	glListBase(_base-32);					// Sets The Base Character to 32	( NEW )
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text	( NEW )
	glPopAttrib();

	glPopMatrix();
}

int RenderModule::Render(){
	// reset model view matrix
	glLoadIdentity();

	// Clear The Screen And The Depth Buffer, stencial buffer will be set to 0
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// make sure the transparent pixels
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	//enable transparency.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// render current scene
	_scene->Render();

	DrawFPS();

	// By using double buffering, we are drawing everything to a hidden screen that we can not see. When we swap the buffer, the screen we see becomes the hidden screen, 
	// and the screen that was hidden becomes visible. This way we don't see our scene being drawn out. It just instantly appears. 
	SwapBuffers(_hDC);

	return 0;
}

void RenderModule::Resize(int $w, int $h){
	std::cout << "Render resize \n";

	// setup viewport
	glViewport(0, 0, $w, $h);

	// start modify projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// orthogonal projection
	glOrtho(0.0f, $w, 0.0f, $h, -1.0f, 1.0f);

	//start model view transformations
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*
	float zoom = 1.0f/2.0f;
	float MTP = 1.7f/32.0f;

	int w = $w;
	int h = $h;

	Vec2f viewCenter = vec2_zero;

	glViewport(0.0f, 0.0f, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	Vec2f extents(w, h);
	extents *= MTP * zoom;

	Vec2f lower = viewCenter - extents;
	Vec2f upper = viewCenter + extents;

	// L/R/B/T
	gluOrtho2D(lower.x, upper.x, lower.y, upper.y);

	//start model view transformations
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	*/
}