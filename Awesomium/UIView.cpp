#include "UIView.h"
#include <GL\GL.h>
#include "UIViewManager.h"

/*
void UIView::BeginNavigationCallback(awe_webview* caller, const awe_string* url, const awe_string* frame_name){
	// update url
	int str_size = awe_string_to_utf8(url, NULL, 0) + 1;
	char* urlChar = new char[str_size];
	awe_string_to_utf8(url, urlChar, str_size);
	UIViewManager::GetInstance()->GetUIView(caller)->url = std::string(urlChar);
}
*/

UIView::UIView(void): _bpp(4)
{

}


UIView::~UIView(void)
{
	awe_webview_destroy(_webview);
}

void UIView::Init( awe_webview* wv, unsigned int w, unsigned int h )
{
	_width = w;
	_height = h;

	_webview = wv;

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

void UIView::Resize( unsigned int w, unsigned int h )
{
	// resize webView
	if(awe_webview_resize(_webview, w, h, false, 500)){
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

void UIView::UpdateBuffer()
{
	// only update the data if it is dirty
	if(awe_webview_is_dirty(_webview)){
		const awe_renderbuffer* renderBuffer = awe_webview_render(_webview);

		// TODO: can be optimized? just update part of the data?
		// update the texture data with the newest render buffer data
		glBindTexture(GL_TEXTURE_2D, _textureID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, awe_renderbuffer_get_buffer(renderBuffer));
	}
}

void UIView::Draw()
{
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, _textureID);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f((GLfloat)_width, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f((GLfloat)_width, (GLfloat)_height, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, (GLfloat)_height, 0.0f);
	glEnd();
}

awe_webview* UIView::GetWebview()
{
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
