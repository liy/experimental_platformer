#pragma once
#include "gl\glew.h"
#include <IL\il.h>
#include <iostream>

void InitGraphicsEngine(){
	// initialize devil loader
	ilInit();
	// All the image loaded will be using top left corner as origin.
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
	ilEnable(IL_ORIGIN_SET);

	// initialize glew, so we can use OpenGL 3.3 functions
	GLenum glewInitResult = glewInit();
	if(glewInitResult != GLEW_OK){
		std::cout << glewGetErrorString(glewInitResult) <<"\n";
	}
}