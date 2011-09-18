/**
 * This is a simple example of using Awesomium with OpenGL.
 *
 * It loads a page and displays it using OpenGL and GLUT.
 */

// Various included headers
#include <Awesomium/awesomium_capi.h>
#include "UIViewManager.h"
#include "UIView.h"

#if defined(__WIN32__) || defined(_WIN32)
#include <windows.h>
#include <gl/gl.h>
#include "gl/freeglut.h"
#define GL_BGRA GL_BGRA_EXT
#elif defined(__APPLE__)
#include <unistd.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#endif

#include <iostream>
#include "AweString.h"

// Various macro definitions
#define WIDTH	1024
#define HEIGHT	768
#define URL	 "E:/GameDev/Platformer/data/ui/editor/generic.html" //"http://www.google.com"
#define INDEX_FILE "index.html"
#define DIR "E:/jquery-ui-1.8.16.custom/"
#define UPDATE_DELAY_MS	5

void cleanup();
void display();
void update(int val);
void mouseMoved(int x, int y);
void mousePressed(int button, int state, int x, int y);
void keyPressed(unsigned char key, int x, int y);
void specialKeyPressed(int key, int x, int y);
void injectSpecialKey(int keyCode);

void webLoadingComplete(awe_webview* caller);


UIViewManager* manager = UIViewManager::GetInstance();
UIView* uiview;

void jsCallback(awe_webview* caller, const awe_string* objectName, const awe_string* callbackName, const awe_jsarray* arguments){
	if(AweString::std_str(objectName) == "jsObject" && AweString::std_str(callbackName) == "trace"){
		const awe_jsvalue* value = awe_jsarray_get_element(arguments, 0);
		const awe_string* trace_string = awe_jsvalue_to_string(value);

		std::cout << "javascript trigger c++ function: " << AweString::std_str(trace_string) << std::endl;
	}

	AweString objName("test");
}

// Our main program
int main(int argc, char *argv[])
{
	// initialize the UIViewManager
	UIViewManager::GetInstance()->Init();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("AwesomiumGL Sample");

	// viewport
	glViewport(0, 0, WIDTH, HEIGHT);


	// Set the correct ortho project.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, WIDTH, 0.0f, HEIGHT, 0.0f, 200.0f);

	// switch to model view matrix
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//set the background colour
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	uiview = new UIView(WIDTH, HEIGHT);
	awe_webview* webview = uiview->GetWebview();
	uiview->LoadURL(URL);


	awe_webview_focus(uiview->GetWebview());

	AweString objName("jsObject");
	awe_webview_create_object(webview, objName.awe_str());
	

	//awe_string* funcName = awe_string_create_from_ascii("trace", strlen("trace"));
	AweString funcName("trace");
	awe_webview_set_object_callback(webview, objName.awe_str(), funcName.awe_str());

	awe_webview_set_callback_js_callback(webview, jsCallback);


	glutDisplayFunc(display);
	glutTimerFunc(UPDATE_DELAY_MS, update, 0);
	glutMouseFunc(mousePressed);
	glutMotionFunc(mouseMoved);
	glutPassiveMotionFunc(mouseMoved);
	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(specialKeyPressed);

	atexit(cleanup);

	glutMainLoop();
	
	return 0;
}

void webLoadingComplete(awe_webview* caller){

	awe_string* urlStr = awe_webview_get_url(caller);

	std::cout << AweString::std_str(urlStr) << "\n";
}

void cleanup()
{
	// Destroy our WebView instance
	//awe_webview_destroy(webView);

	// Destroy our WebCore instance
	// NOTE: Since we don't have access to the main loop of
	// our application (a limitation of GLUT), we scheduled
	// this cleanup() method using atexit. The problem is
	// that Awesomium schedules some items for cleanup using
	// atexit as well and their order of execution is not
	// guaranteed which may cause a crash to occur.
	//
	// The Solution: Don't use GLUT or atexit :-)
	//awe_webcore_shutdown();

	delete manager;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	manager->UpdateViewBuffer();
	manager->Draw();

	glutSwapBuffers();

	GLenum ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		std::cout << " error";
	}
}

void update(int val)
{
	awe_webcore_update();

	

	// Call our display func when the WebView needs rendering
	if(awe_webview_is_dirty(uiview->GetWebview())){
		glutPostRedisplay();
	}

	glutTimerFunc(UPDATE_DELAY_MS, update, 0);
}

void mouseMoved(int x, int y)
{
	awe_webview_inject_mouse_move(uiview->GetWebview(), x, y);
}

void mousePressed(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_DOWN)
			awe_webview_inject_mouse_down(uiview->GetWebview(), AWE_MB_LEFT);
		else
			awe_webview_inject_mouse_up(uiview->GetWebview(), AWE_MB_LEFT);
	}
}


void keyPressed(unsigned char key, int x, int y)
{
	if(key == 8 || key == 127) // Backspace or Delete key
	{
		injectSpecialKey(VK_BACK);
		return;
	}
	else if(key == 9) // Tab key
	{
		injectSpecialKey(VK_TAB);
		return;
	}
	else if(key == 13){
		// enter key
		injectSpecialKey(13);
		return;
	}
	else if(key == 27) // Escape key
	{
		exit(0);
	}

	// injection of normal character keys
	awe_webkeyboardevent e;
	e.type = AWE_WKT_CHAR;
	e.is_system_key = false;
	e.text[0] = key;
	e.text[1] = 0;
	e.text[2] = 0;
	e.text[3] = 0;
	e.unmodified_text[0] = 0;
	e.unmodified_text[1] = 0;
	e.unmodified_text[2] = 0;
	e.unmodified_text[3] = 0;
	e.virtual_key_code = 0;
	e.native_key_code = 0;
	awe_webview_inject_keyboard_event(uiview->GetWebview(), e);
}

void specialKeyPressed(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_LEFT:
		injectSpecialKey(VK_LEFT);
		break;
	case GLUT_KEY_UP:
		injectSpecialKey(VK_UP);
		break;
	case GLUT_KEY_RIGHT:
		injectSpecialKey(VK_RIGHT);
		break;
	case GLUT_KEY_DOWN:
		injectSpecialKey(VK_DOWN);
		break;
	case GLUT_KEY_PAGE_UP:
		injectSpecialKey(VK_PRIOR);
		break;
	case GLUT_KEY_PAGE_DOWN:
		injectSpecialKey(VK_NEXT);
		break;
	case GLUT_KEY_HOME:
		injectSpecialKey(VK_HOME);
		break;
	case GLUT_KEY_END:
		injectSpecialKey(VK_END);
		break;
	}
}

void injectSpecialKey(int keyCode)
{
	// Key Down
	awe_webkeyboardevent e;
	e.is_system_key = false;
	e.modifiers = 0;

	e.text[0] = keyCode;
	e.text[1] = 0;
	e.text[2] = 0;
	e.text[3] = 0;
	e.unmodified_text[0] = keyCode;
	e.unmodified_text[1] = 0;
	e.unmodified_text[2] = 0;
	e.unmodified_text[3] = 0;
	e.virtual_key_code = keyCode;
	e.native_key_code = keyCode;
	e.type = AWE_WKT_KEYDOWN;
	awe_webview_inject_keyboard_event(uiview->GetWebview(), e);

	// Key Up
	e.type = AWE_WKT_KEYUP;
	awe_webview_inject_keyboard_event(uiview->GetWebview(), e);

	// update immediately
	awe_webcore_update();
}