#pragma once

#include <string>
#include <Awesomium/awesomium_capi.h>

typedef unsigned int GLuint;
typedef unsigned char GLubyte;

class UIView
{
	/**
	 * Only the manager can create UIView.
	 */
	friend class UIViewManager;

public:
	/**
	 *	
	 */
	virtual void Init(awe_webview* wv, unsigned int w, unsigned int h);

	/**
	 *	
	 */
	virtual void Resize(unsigned int w, unsigned int h);

	/**
	 * 
	 */
	const unsigned int GetWidth() const;

	/**
	 * 
	 */
	const unsigned int GetHeight() const;

	/**
	 * 
	 */
	awe_webview* GetWebview();

	/**
	 * 
	 */
	GLuint GetTextureID();

	// TODO: to be removed. Change it into local variable.
	/**
	 * 
	 */
	GLubyte* buffer;

protected:
	UIView(void);
	virtual ~UIView(void);

	/**
	 * 
	 */
	virtual void UpdateBuffer();

	/**
	 * 
	 */
	virtual void Draw();

	GLuint _textureID;
	awe_webview* _webview;
	unsigned int _bpp, _width, _height;
};

