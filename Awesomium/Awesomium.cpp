/**
 * This is a simple example of using Awesomium with OpenGL.
 *
 * It loads a page and displays it using OpenGL and GLUT.
 */

// Various included headers
#include <Awesomium/awesomium_capi.h>
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

// Various macro definitions
#define WIDTH	1024
#define HEIGHT	768
#define URL	"http://www.google.com"
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

static awe_webview* webView = 0;

// Our main program
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("AwesomiumGL Sample");

	// Initialize OpenGL
	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WIDTH, 0, HEIGHT);

	// Create our WebCore singleton with the default options
	awe_webcore_initialize_default();
	
	awe_string* base_dir = awe_string_create_from_ascii(DIR, strlen(DIR));
	awe_webcore_set_base_directory(base_dir);

	// Create a new WebView instance with a certain width and height, using the 
	// WebCore we just created
	webView = awe_webcore_create_webview(WIDTH, HEIGHT, false);

	awe_webview_set_transparent(webView, true);
	
	// Load a certain URL into our WebView instance
	//awe_string* url_str = awe_string_create_from_ascii(URL, strlen(URL));
	//awe_webview_load_url(webView, url_str, awe_string_empty(), awe_string_empty(), awe_string_empty());

	
	awe_string* file_str = awe_string_create_from_ascii(INDEX_FILE, strlen(INDEX_FILE));
	awe_webview_load_file(webView, file_str, awe_string_empty());

	// destroy string
	//awe_string_destroy(url_str);
	awe_string_destroy(base_dir);
	awe_string_destroy(file_str);

	awe_webview_focus(webView);

	awe_webview_set_callback_finish_loading(webView, webLoadingComplete);

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

	int str_size = awe_string_to_utf8(urlStr, NULL, 0) + 1;
	char* url = new char[str_size];
	awe_string_to_utf8(urlStr, url, str_size);

	std::cout << url << "\n";
}

void cleanup()
{
	// Destroy our WebView instance
	awe_webview_destroy(webView);

	// Destroy our WebCore instance
	// NOTE: Since we don't have access to the main loop of
	// our application (a limitation of GLUT), we scheduled
	// this cleanup() method using atexit. The problem is
	// that Awesomium schedules some items for cleanup using
	// atexit as well and their order of execution is not
	// guaranteed which may cause a crash to occur.
	//
	// The Solution: Don't use GLUT or atexit :-)
	awe_webcore_shutdown();
}

void display()
{
	

	glClear(GL_COLOR_BUFFER_BIT);

	// Flip image vertically
	glRasterPos2i(0, HEIGHT); 
	glPixelZoom(1.0f,-1.0f); 

	const awe_renderbuffer* renderBuffer = awe_webview_render(webView);

	if(renderBuffer != NULL)
	{

		// Draw pixels directly to screen from our image buffer
		glDrawPixels(WIDTH, HEIGHT, GL_BGRA, GL_UNSIGNED_BYTE, awe_renderbuffer_get_buffer(renderBuffer));
	}

	glutSwapBuffers();
}

void update(int val)
{
	awe_webcore_update();

	// Call our display func when the WebView needs rendering
	if(awe_webview_is_dirty(webView))
		glutPostRedisplay();

	glutTimerFunc(UPDATE_DELAY_MS, update, 0);
}

void mouseMoved(int x, int y)
{
	awe_webview_inject_mouse_move(webView, x, y);
}

void mousePressed(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_DOWN)
			awe_webview_inject_mouse_down(webView, AWE_MB_LEFT);
		else
			awe_webview_inject_mouse_up(webView, AWE_MB_LEFT);
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
	awe_webview_inject_keyboard_event(webView, e);
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
	awe_webview_inject_keyboard_event(webView, e);

	// Key Up
	e.type = AWE_WKT_KEYUP;
	awe_webview_inject_keyboard_event(webView, e);

	// update immediately
	awe_webcore_update();
}