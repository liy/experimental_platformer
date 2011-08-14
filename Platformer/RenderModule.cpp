#include "RenderModule.h"
#include "ATexture.h"
#include "ATextureManager.h"
#include "ASprite.h"
#include "Game.h"
#include "Scene.h"
#include <sstream>
#include "AShaderManager.h"
#include "AShader.h"

RenderModule::RenderModule(void)
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
	glPolygonMode(GL_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	//enable transparency.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//set the background colour
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// by default use orthogonal projection.
	UseOrthogonal();

	InitDefaultShaders();

	return 0;
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

void RenderModule::InitDefaultShaders(){
	AShaderManager* shaderManager = AShaderManager::GetInstance();
	// create a normal shader program
	shaderManager->CreateShaderProgram("default");

	// attach the normal vertex shader
	shaderManager->CreateShader("default.vert", "..\\Shaders\\default.vert", Vertex);
	shaderManager->AttachShader("default.vert", "default");

	// attach the normal fragment shader
	shaderManager->CreateShader("default.frag", "..\\Shaders\\default.frag", Fragment);
	shaderManager->AttachShader("default.frag", "default");

	// activate the normal shader program
	//shaderManager->ActivateProgram("default");

	GLenum ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: Could not create the shaders: %s \n",
			gluErrorString(ErrorCheckValue)
			);

		exit(-1);
	}
}