#include "UIView.h"
#include "UIViewManager.h"
#include <iostream>
#include <Awesomium/awesomium_capi.h>
#include <GL\GL.h>

void BeginNavigationCallback(awe_webview* caller, const awe_string* url, const awe_string* frame_name){
	// update url
	int str_size = awe_string_to_utf8(url, NULL, 0) + 1;
	char* urlChar = new char[str_size];
	awe_string_to_utf8(url, urlChar, str_size);
	std::cout << std::string(urlChar) << "\n";
}

void LoadCompleteCallback(awe_webview* caller){
	std::cout << "loading complete" << "\n";
}

UIView::UIView(unsigned int w, unsigned int h): _bpp(4)
{
	_width = w;
	_height = h;

	// keep a reference of the webview so later we can retrieve it directly from this UIView instance.
	_webview = awe_webcore_create_webview(_width, _height, false);

	// add some callback function
	awe_webview_set_callback_begin_navigation(_webview, BeginNavigationCallback);
	awe_webview_set_callback_finish_loading(_webview, LoadCompleteCallback);

	// create a data buffer for drawing the web view
	buffer = new GLubyte[_width * _height * _bpp];

	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);

	// Select modulate to make tint effect when supply color information
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// Setup the scale up scale down interpolation method.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Setup the wrapping method.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	// transparent is always enabled.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, buffer);

	// Add this UIView to the manager.
	UIViewManager::GetInstance()->AddUIView(this);
}

UIView::~UIView(void)
{
	glDeleteTextures(1, &_textureID);
	delete[] buffer;

	awe_webview_destroy(_webview);
}

void UIView::UpdateBuffer()
{
	// only update the data if it is dirty
	if(awe_webview_is_dirty(_webview)){
		const awe_renderbuffer* renderBuffer = awe_webview_render(_webview);

		memcpy(buffer, awe_renderbuffer_get_buffer(renderBuffer), _width * _height * 4);

		// TODO: can be optimized? just update part of the data?
		// update the texture data with the newest render buffer data
		glBindTexture(GL_TEXTURE_2D, _textureID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, buffer);
	}
}

void UIView::Draw()
{
	/*
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	// Enable the texture 2d state in order to draw texture!
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureID);

	glColor4f(0.8f, 0.0f, 0.0f, 1.0f);

	// TODO: immediate mode. To be removed completely, it should use shader OpenGL3.1+ approach.
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f((GLfloat)_width, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex2f((GLfloat)_width, (GLfloat)_height);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, (GLfloat)_height);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	*/
}

void UIView::LoadURL( const std::string& url )
{
	awe_string* awe_str_url = awe_string_create_from_ascii(url.c_str(), strlen(url.c_str()));
	awe_webview_load_url(_webview, awe_str_url, awe_string_empty(), awe_string_empty(), awe_string_empty());
}

awe_webview* UIView::GetWebview()
{
	awe_webview_get_url(_webview);
	return _webview;
}

const unsigned int UIView::GetWidth() const
{
	return _width;
}

const unsigned int UIView::GetHeight() const
{
	return _height;
}

GLuint UIView::GetTextureID()
{
	return _textureID;
}

void UIView::Resize( unsigned int w, unsigned int h )
{
	// resize webView
	if(awe_webview_resize(_webview, w, h, false, 500)){
		glDeleteTextures(1, &_textureID);
		delete[] buffer;

		_width = w;
		_height = h;

		// create a data buffer for drawing the web view
		buffer = new GLubyte[_width * _height * _bpp];

		glGenTextures(1, &_textureID);
		glBindTexture(GL_TEXTURE_2D, _textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		// always enable transparent 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, buffer);
	}
}