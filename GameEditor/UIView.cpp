#include "UIView.h"
#include <typeinfo>
#include "gl\glew.h"
#include "UIViewManager.h"
#include <iostream>
#include <Awesomium/awesomium_capi.h>
#include "TransMatrices.h"
#include "AShaderManager.h"
#include "ATexture.h"
#include "ATextureBinder.h"

UIView::UIView(unsigned int w, unsigned int h)
{
	_texture = new ATexture(w, h, GL_BGRA_EXT, 4);

	// keep a reference of the webview so later we can retrieve it directly from this UIView instance.
	_webview = awe_webcore_create_webview(w, h, false);

	// add some callback function, the manager will take care of finding correct corresponding UIView and notify its listeners.
	awe_webview_set_callback_js_callback(_webview, UIViewManager::OnCallback);
	awe_webview_set_callback_begin_loading(_webview, UIViewManager::OnBeginLoading);
	awe_webview_set_callback_begin_navigation(_webview, UIViewManager::OnBeginNavigation);
	awe_webview_set_callback_finish_loading(_webview, UIViewManager::OnFinishLoading);
	awe_webview_set_callback_change_cursor(_webview, UIViewManager::OnChangeCursor);
	awe_webview_set_callback_open_external_link(_webview, UIViewManager::OnOpenExternalLink);
	awe_webview_set_callback_request_file_chooser(_webview, UIViewManager::OnRequestFileChooser);

	// Add this UIView to the manager.
	UIViewManager::GetInstance()->AddUIView(this);

	_indices = new GLubyte[4];
	_indices[0] = 3;
	_indices[1] = 0;
	_indices[2] = 2;
	_indices[3] = 1;

	_vertices = new Vertex3f[4];
	_vertices[0].colour.Set(255, 255, 255, 255);
	_vertices[1].colour.Set(255, 255, 255, 255);
	_vertices[2].colour.Set(255, 255, 255, 255);
	_vertices[3].colour.Set(255, 255, 255, 255);

	SetRect(0, 0, w, h);

	CreateVBO();
}

UIView::~UIView(void)
{
	// Remove the this UIView instance from the map.
	UIViewManager::GetInstance()->RemoveUIView(this);

	// TODO: check if the texture is stored in the cache?? At the moment, the dynamic generated texture will not be stored in the cache. Probably in the future, it will be.

	// Release the texture memory in the graphics card. 
	const GLuint id = _texture->GetTextureID();
	glDeleteTextures(1, &id);

	// Destroy the texture object.
	delete _texture;

	// Destroy the webview.
	awe_webview_destroy(_webview);
}

void UIView::UpdateBuffer()
{
	// only update the data if it is dirty
	if(awe_webview_is_dirty(_webview) && !awe_webview_is_loading_page(_webview)){
		const awe_renderbuffer* renderBuffer = awe_webview_render(_webview);

		// TODO: can be optimized? just update part of the data?
		// update the texture data with the newest render buffer data
		ATextureBinder::GetInstance()->Bind(_texture->GetTextureID());
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _texture->contentWidth(), _texture->contentHeight(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, awe_renderbuffer_get_buffer(renderBuffer));
	}
}

void UIView::Draw()
{
	if(!awe_webview_is_loading_page(_webview)){
		// save the current model view matrix
		TransMatrices* matrices = TransMatrices::Instance();
		matrices->Push();

		// concatenate the transformation
		matrices->modelView.SetIdentity();

		// set the vertex shader's model view matrix ready for drawing.
		GLuint modelViewUnifo = glGetUniformLocation(AShaderManager::GetInstance()->activatedProgramID, "modelView");
		glUniformMatrix4fv(modelViewUnifo, 1, GL_FALSE, matrices->modelView);

		// bind the vertex states
		glBindVertexArray(_vaoID);
		// bind texture
		ATextureBinder::GetInstance()->Bind(_texture->GetTextureID());
		// draw
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, 0);
		// unbind the vertex state
		glBindVertexArray(0);

		// restore original model view matrix.
		matrices->Pop();
	}

	GLenum ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"UIView OpenGL error: %s \n",
			gluErrorString(ErrorCheckValue)
			);
	}
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
	return _texture->width();
}

const unsigned int UIView::GetHeight() const
{
	return _texture->height();
}

const GLuint UIView::GetTextureID() const
{
	return _texture->GetTextureID();
}

void UIView::Resize( unsigned int w, unsigned int h )
{
	// resize webView
	if(awe_webview_resize(_webview, w, h, false, 500)){
		delete _texture;

		_texture = new ATexture(w, h, GL_BGRA_EXT, 4);

		// resize then update the vertices information
		SetRect(0, 0, w, h);
	}
}

void UIView::CreateVBO(void)
{
	int vertexSize = sizeof(Vertex3f);
	int bufferSize = vertexSize * sizeof(_indices)/sizeof(GLubyte);

	// initialization for VAO
	// The vertex array basically is used for saving all the states. So later when drawing, 
	//  instead of bind texture, setup texture coordinate, colour information, etc. we can simply use glBindVertexArray(_vaoID) to set the state and ready for drawing.
	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);



	// create the vbo
	glGenBuffers(1, &_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, _vertices, GL_STATIC_DRAW);

	// This points the vertex position to vertex shader's  location 0 variable. 
	// This points the colour to vertex shader's location 1 variable.
	// Points the texture coordinate to location 2 variable
	// TODO: comment needs rephrase
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// since we are using the VBO, the glBufferData already copied the data into graphic card's memory
	// and the pointer pointed at the started of the memory, so no need to get the _vertice's memory.
	int startAddr = 0;
	// texture
	int offset = 0;	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)startAddr);
	// vertex position information
	offset = offsetof(Vertex3f, v);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, vertexSize, (void*)(startAddr+offset));
	// vertex colour information
	offset = offsetof(Vertex3f, colour);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexSize, (void*)(startAddr+offset));

	// create ibo
	glGenBuffers(1, &_iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);

	// unbind the vao
	glBindVertexArray(0);

	GLenum ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: %s could not create a VBO: %s \n",
			typeid(this).name(),
			gluErrorString(ErrorCheckValue)
			);

		//exit(-1);
	}
}

void UIView::SetRect(int $x, int $y, int $width, int $height){
	_rect.x = $x;
	_rect.y = $y;
	_rect.width = $width;
	_rect.height = $height;

	_vertices[0].v.Set(0.0f, 0.0f, 0.0f);
	_vertices[1].v.Set(_rect.width, 0.0f, 0.0f);
	_vertices[2].v.Set(_rect.width, _rect.height, 0.0f);
	_vertices[3].v.Set(0.0f, _rect.height, 0.0f);

	// calculate bottom left of the image in texture coordinate. 
	float u = (float)_rect.x/_texture->width();
	float v = (float)_rect.y/_texture->height();
	// Calculate the the width and height in texture coordinate.
	float w = (float)_rect.width/_texture->width();
	float h = (float)_rect.height/_texture->height();

	// assign the texture coordinate
	_vertices[3].uv.Set(u, v);
	_vertices[2].uv.Set(u + w, v);
	_vertices[1].uv.Set(u + w, v + h);
	_vertices[0].uv.Set(u, v + h);
}

std::vector<UIViewListener*> UIView::GetListeners()
{
	return _listeners;
}

void UIView::AddListener( UIViewListener* listener )
{
	_listeners.push_back(listener);
}

void UIView::RemoveListener( UIViewListener* listener )
{
	for(int i=0; i<_listeners.size(); ++i){
		// FIXME: Are you sure this will work??
		if(_listeners[i] == listener){
			_listeners.erase(_listeners.begin()+i);
			break;
		}
	}
}

void UIView::Focus()
{
	UIViewManager::GetInstance()->Focus(this);
}

void UIView::Unfocus()
{
	UIViewManager::GetInstance()->Unfocus(this);
}
