#include "RenderModule.h"
#include "ATexture.h"
#include "ATextureManager.h"
#include "AImage.h"
#include "Game.h"
#include "Scene.h"
#include <sstream>

RenderModule::RenderModule(void): _fps(0)
{

}


RenderModule::~RenderModule(void)
{
	_game = NULL;
	_scene = NULL;
}

int RenderModule::Init(Game* $game, HDC& $hDC, unsigned int sw, unsigned int sh){
	std::cout << "Render init\n";
	_hDC = $hDC;

	Resize(sw, sh);

	_game = $game;
	_scene = &_game->getCurrentScene();

	// do not render the back of the texture.
	//glPolygonMode(GL_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	//enable transparency.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//set the background colour
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// by default use orthogonal projection.
	UseOrthogonal();

	return 0;
}

void RenderModule::DrawString(int x, int y, const char *str, ...){
	char buffer[128];

	va_list arg;
	va_start(arg, str);
	vsprintf(buffer, str, arg);
	va_end(arg);

	glPushMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos3i(x, y, 0);
	int length = (int)strlen(buffer);
	for (int i = 0; i < length; ++i){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, buffer[i]);
	}

	glPopMatrix();
}

void RenderModule::DrawFPS(){

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

	// conver to string
	std::stringstream ss;
	ss << _fps;

	// draw the string
	DrawString(-_screenWidth/2 + 5, _screenHeight/2 - 12 - 5, ss.str().c_str());
}

int RenderModule::Render(){
	// reset model view matrix
	glLoadIdentity();

	// Clear The Screen And The Depth Buffer, stencial buffer will be set to 0
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// make sure the transparent pixels
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//enable transparency.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	DrawFPS();

	// render current scene
	// reset the matrix
	glLoadIdentity();
	_scene->Render();

	// By using double buffering, we are drawing everything to a hidden screen that we can not see. When we swap the buffer, the screen we see becomes the hidden screen, 
	// and the screen that was hidden becomes visible. This way we don't see our scene being drawn out. It just instantly appears. 
	SwapBuffers(_hDC);

	return 0;
}

void RenderModule::UsePerspective(){
	// start modify projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Set the correct ortho project.
	//glOrtho(-$w/2.0f, $w/2.0f, -$h/2.0f, $h/2.0f, 0.0f, 200.0f);

	gluPerspective(60.0f, _screenWidth/_screenHeight, 0.0f, 200.0f);

	//start model view transformations
	glMatrixMode(GL_MODELVIEW);
}

void RenderModule::UseOrthogonal(){
	// start modify projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Set the correct ortho project.
	//glOrtho(-$w/2.0f, $w/2.0f, -$h/2.0f, $h/2.0f, 0.0f, 200.0f);

	glOrtho(0.0f, _screenWidth, 0.0f, _screenHeight, 0.0f, 200.0f);

	//start model view transformations
	glMatrixMode(GL_MODELVIEW);
}

void RenderModule::Resize(unsigned int sw, unsigned int sh){
	_screenWidth = sw;
	_screenHeight = sh;

	// setup viewport
	glViewport(0, 0, _screenWidth, _screenHeight);
}