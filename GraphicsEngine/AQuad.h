#pragma once

#include <GL\glut.h>
#include <GL\GL.h>

class AQuad
{
public:
	AQuad(void);
	~AQuad(void);

	float x;
	float y;

	float width;
	float height;

	float r;
	float g;
	float b;
	float a;

	void Draw();

};

