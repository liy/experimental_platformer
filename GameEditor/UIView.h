#pragma once

#include "AGeom.h"
#include <string>
#include <windows.h>
#include <map>

typedef unsigned int GLuint;
typedef unsigned char GLubyte;
typedef struct _awe_webview awe_webview;

class ATexture;

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
	const GLuint UIView::GetTextureID() const;

	/**
	 * 
	 */
	virtual void SetRect(int $x, int $y, int $width, int $height);

protected:
	/**
	 * 
	 */
	virtual void CreateVBO();

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
	awe_webview* _webview;

	/**
	 * 
	 */
	Vertex3f* _vertices;

	/**
	 * 
	 */
	GLubyte* _indices;

	//
	GLuint _vaoID, _vboID, _iboID;

	/**
	 * 
	 */
	ATexture* _texture;

	/**
	 * 
	 */
	Recti _rect;
};

