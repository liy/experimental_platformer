#pragma once

#include "AGeom.h"
#include <string>
#include <windows.h>
#include <map>
#include <vector>

typedef unsigned int GLuint;
typedef unsigned char GLubyte;
typedef struct _awe_webview awe_webview;

class ATexture;
class UIViewListener;

/**
 * @class	UIView
 *
 * @brief	This is a wrapper class for awe_webview in Awesomium.
 */
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

	/**
	 * @fn	virtual void UIView::AddListener(const UIViewListener* listener);
	 *
	 * @brief	Adds a listener. 
	 *
	 * @param	listener	The listener.
	 */
	virtual void AddListener(UIViewListener* listener);

	/**
	 * @fn	virtual void UIView::RemoveListener(const UIViewListener* listener);
	 *
	 * @brief	Removes the listener described by listener.
	 *
	 * @param	listener	The listener.
	 */
	virtual void RemoveListener(UIViewListener* listener);

	/**
	 * @fn	virtual std::vector<UIViewListener*> UIView::GetListeners();
	 *
	 * @brief	Gets the listeners.
	 *
	 * @return	null if it fails, else the listeners.
	 */
	virtual std::vector<UIViewListener*> GetListeners();

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


	std::vector<UIViewListener*> _listeners;
};
