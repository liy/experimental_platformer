#include "RenderModule.h"
#include "ATexture.h"
#include "ATextureManager.h"
#include "AImage.h"
#include "Game.h"
#include "Scene.h"
#include <sstream>

RenderModule::RenderModule(HDC $hDC): _hDC($hDC), _fps(0)
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

	// do not render the back of the texture.
	glPolygonMode(GL_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	//enable transparency.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//set the background colour
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	return 0;
}

void RenderModule::DrawString(int x, int y, const char *str, ...){
	char buffer[128];

	va_list arg;
	va_start(arg, str);
	vsprintf(buffer, str, arg);
	va_end(arg);

	int w = _game->screenWidth();
	int h = _game->screenHeight();

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
	DrawString(-_game->screenWidth()/2 + 5, _game->screenHeight()/2 - 12 - 5, ss.str().c_str());
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

	DrawFPS();

	// render current scene
	_scene->Render();


	// By using double buffering, we are drawing everything to a hidden screen that we can not see. When we swap the buffer, the screen we see becomes the hidden screen, 
	// and the screen that was hidden becomes visible. This way we don't see our scene being drawn out. It just instantly appears. 
	SwapBuffers(_hDC);

	return 0;
}

void RenderModule::SetupProjection(int $w, int $h){
	Resize($w, $h);
}

void RenderModule::Resize(int $w, int $h){
	std::cout << "Render resize \n";

	// setup viewport
	glViewport(0, 0, $w, $h);

	// start modify projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Set the correct ortho project.
	//glOrtho(-$w/2.0f, $w/2.0f, -$h/2.0f, $h/2.0f, 0.0f, 200.0f);

	glOrtho(0.0f, $w, 0.0f, $h, 0.0f, 200.0f);

	//start model view transformations
	glMatrixMode(GL_MODELVIEW);
	// reset the module view matrix
	glLoadIdentity();
}