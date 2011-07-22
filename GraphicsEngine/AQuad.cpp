#include "AQuad.h"


AQuad::AQuad(void): x(0.0f), y(0.0f), width(32.0f), height(32.0f), r(1.0f), g(0.0f), b(0.0f), a(0.7f)
{

}


AQuad::~AQuad(void)
{

}


void AQuad::Draw(){
	glColor4f(r, g, b, a);

	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glLineWidth(1.0f);
	glBegin(GL_LINE_LOOP);
		glVertex2d(x, y+height);
		glVertex2d(x+width, y+height);
		glVertex2d(x+width, y);
		glVertex2d(x, y);
	glEnd();

	//glDisable(GL_LINE_SMOOTH);
}