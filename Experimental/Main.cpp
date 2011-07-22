#pragma once
#include "ATextureManager.h"
#include "AImage.h"
#include <GL\glut.h>
#include <GL\GL.h>

float angle = 0.0f;
float red=1.0f, blue=1.0f, green=1.0f;

AImage* image;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio =  w * 1.0 / h;

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the correct perspective.
	//gluPerspective(90.0f, ratio, 0.1f, 2000.0f);

	// Set the correct perspective.
	glOrtho(-w/2.0f, w/2.0f, -h/2.0f, h/2.0f, -70.0f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {
	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
	// reset model view matrix
	glLoadIdentity();

	// Clear The Screen And The Depth Buffer, stencial buffer will be set to 0
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// make sure the transparent pixels
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	//enable transparency.
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//image->Draw(vec2_zero, 0);

	glRotatef(angle, 0.0f, 1.0f, 0.0f);

	glBegin(GL_POLYGON);
	glVertex3f(0.0f, 100.0f, 0.0f);
	glVertex3f(-80.0f, -100.0f, 0.0f);
	glVertex3f(80.0f, -100.0f, 0.0f);
	glEnd();


	angle+=1.4f;

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27)
		exit(0);
}

void processSpecialKeys(int key, int x, int y) {

	switch(key) {
		case GLUT_KEY_F1 :
				red = 1.0;
				green = 0.0;
				blue = 0.0; break;
		case GLUT_KEY_F2 :
				red = 0.0;
				green = 1.0;
				blue = 0.0; break;
		case GLUT_KEY_F3 :
				red = 0.0;
				green = 0.0;
				blue = 1.0; break;
		case GLUT_KEY_UP:
			break;
		case GLUT_KEY_DOWN:
			break;
		case GLUT_KEY_LEFT:
			break;
		case GLUT_KEY_RIGHT:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;
		case GLUT_KEY_PAGE_UP:
			break;
	}
}

void Init(){
	glPolygonMode(GL_BACK, GL_LINE);

	// You must initialize the TextureManager first before use.
	ATextureManager::GetInstance()->Init();

	image = new AImage("screen.jpg");
}

void main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1024,768);
	glutCreateWindow("OpenGL test");

	Init();

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	// here are the new entries
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);

	// enter GLUT event processing cycle
	glutMainLoop();
}