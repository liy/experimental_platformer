#pragma once

#include <string>

typedef unsigned int GLuint;
typedef unsigned char GLubyte;
typedef struct _awe_webview awe_webview;

class UIView
{
	/**
	 * Only the manager can create UIView.
	 */
	friend class UIViewManager;

public:
	/**
	 * Before you can create UIView instance, you must first initialize the UIViewManager first!
	 */
	UIView(unsigned int w, unsigned int h);

	/**
	 * 
	 */
	virtual ~UIView(void);

	/**
	 * 
	 */
	virtual void LoadURL(const std::string& url);

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

	/**
	 * 
	 */
	virtual void UpdateBuffer();



	/**
	 * 
	 */
	virtual void Draw();

	/**
	 * 
	 */
	GLuint _textureID;

	/**
	 * 
	 */
	awe_webview* _webview;

	/**
	 * 
	 */
	unsigned int _bpp;

	/**
	 * 
	 */
	unsigned int _width;

	/**
	 * 
	 */
	unsigned int _height;
};

