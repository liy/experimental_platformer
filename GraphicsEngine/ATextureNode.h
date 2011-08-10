#pragma once
#include <windows.h>
#include <gl\glu.h>
#include "AGeom.h"
#include <map>
#include "ATextureManager.h"
#include "ATexture.h"

/**
 *	Abstract class 
 */
class ATextureNode
{
private:
	void Init(const Recti& $rect){
		// initialize the vertices
		_vertices = new Vertex3f[4];
		_vertices[0].colour.Set(255, 0, 0, 255);
		_vertices[1].colour.Set(255, 0, 0, 255);
		_vertices[2].colour.Set(255, 0, 0, 255);
		_vertices[3].colour.Set(255, 0, 0, 255);

		// assign the texture rectangle. The vertices position will be initialized as well.
		SetRect($rect);

		// setup default scale and anchor position, for transformation use
		_scale.Set(1.0f, 1.0f);
		_anchorRatio.Set(0.5f, 0.5f);
		_rotation = 0.0f;
		_position.SetZero();

		// initialize the transformation matrix.
		_transform = mat4_identity;
		// (translation *  rotation * scale * anchorTranslation) * Vertex
		_transform.Translate(_position.x, _position.y, _position.z);
		_transform.RotateZ(_rotation);
		_transform.Scale(_scale.x, _scale.y, 1.0f);
		Vec2f anchorTrans = GetAnchor();
		_transform.Translate(-anchorTrans.x, -anchorTrans.y, 0.0f);

		// initialize the indices array
		_indices = new GLubyte[6];
		_indices[0] = 0;
		_indices[1] = 1;
		_indices[2] = 3;
		_indices[3] = 3;
		_indices[4] = 1;
		_indices[5] = 2;
	}
public:
	ATextureNode(void){
		Init(Recti(1.0f, 0.0f, 1.0f, 1.0f));
	}

	ATextureNode(const std::string& $fileName){
		// initialize the texture
		_texture_sp = ATextureManager::GetInstance()->Get($fileName);

		// assign the texture rectangle. The vertices position will be initialized as well.
		Init(Recti(1.0f, 0.0f, _texture_sp->contentWidth(), _texture_sp->contentHeight()));
	}

	ATextureNode(const std::string& $fileName, const Recti& $rect){
		// initialize the texture
		_texture_sp = ATextureManager::GetInstance()->Get($fileName);

		Init($rect);
	};

	virtual ~ATextureNode(void){
		// FIXME how to free the image texture coordinate????

		// keep a copy record of the filename, to remove the 
		std::string fileName = _texture_sp->fileName();
		std::cout << "AImage["<< fileName <<"] destroy!\n";
		// Null the reference, so we can try to remove th texture
		_texture_sp = NULL;
		// Try to remove the using texture from the memory.
		// If the reference count is 1.(1 reference count is maintained by the map). Then we remove it from the memory.
		// So programmer will not need to manually  remove texture from the memory
		ATextureManager::GetInstance()->Remove(fileName);
	};

	// update the texture this image is using.
	void SetTexture(const std::string& $fileName, const Recti& $rect){
		// Dynamically change texture, we have to try to remove the previous texture it was using.
		if(_texture_sp != NULL && _texture_sp->fileName() != $fileName){
			std::string fileName = _texture_sp->fileName();
			// Null the reference, so we can try to remove th texture
			_texture_sp = NULL;
			// Try to remove the using texture from the memory.
			ATextureManager::GetInstance()->Remove(fileName);
		}

		// update to the new texture
		_texture_sp = ATextureManager::GetInstance()->Get($fileName);
		// update texture coordinate
		SetRect($rect);
	}

	// update the texture this image is using.
	void SetTexture(const std::string& $fileName){
		// Dynamically change texture, we have to try to remove the previous texture it was using.
		if(_texture_sp != NULL && _texture_sp->fileName() != $fileName){
			std::string fileName = _texture_sp->fileName();
			// Null the reference, so we can try to remove th texture
			_texture_sp = NULL;
			// Try to remove the using texture from the memory.
			ATextureManager::GetInstance()->Remove(fileName);
		}

		// update to the new texture
		_texture_sp = ATextureManager::GetInstance()->Get($fileName);
		// update texture coordinate
		SetRect(0, 0, _texture_sp->contentWidth(), _texture_sp->contentHeight());
	}

	// Get the currently using texture file name.
	virtual const std::string& fileName() const{
		return _texture_sp->fileName();
	}

	// Update the graphic, this probably only apply to the animation. For static image, this function does nothing
	virtual void Update(unsigned short delta){};

	// Draw the image to a specific position and rotation
	virtual void Draw(const Vec3f& position, float rotation){
		Draw(position.x, position.y, position.z, rotation);
	}

	// Draw the image to a specific position and rotation
	virtual void Draw(const Vec2f& position, float z, float rotation){
		Draw(position.x, position.y, z, rotation);
	}

	// Draw the image to a specific position and rotation
	virtual void Draw(float x, float y, float z, float rotation) =0;

	// Draw the image using a specific matrix transformation
	virtual void Draw(const Mat4f& mat) =0;

	virtual void Draw() =0;

	/**
	 *	Set the rotation of the graphics
	 */
	virtual void SetRotation(float r){
		_rotation = r;

		// rest the transformation matrix to identity matrix
		_transform = Mat4f();

		// (translation *  rotation * scale * anchorTranslation) * Vertex
		_transform.Translate(_position.x, _position.y, _position.z);
		_transform.RotateZ(_rotation);
		_transform.Scale(_scale.x, _scale.y, 1.0f);
		// TODO: anchor variables need changing
		_transform.Translate(-(float)(_rect.width)*_anchorRatio.x, -(float)(_rect.height)*_anchorRatio.y, 0.0f);
	}

	/**
	 *	Get the rotation of the graphics
	 */
	virtual const float GetRotation() const{
		return _rotation;
	}

	virtual void SetPosition(float x, float y, float z=0.0f){
		_position.Set(x, y, z);
		
		// clear the translation elements.
		_transform[12] = 0.0f;
		_transform[13] = 0.0f;
		_transform[14] = 0.0f;
		
		// I assume that the current transformation matrix's translation is only affect on the translation elements.
		// (Of course, if you manually  set the transformation matrix, the translation could affect on other elements as well.)
		//
		// The only transformations left according to the transformation sequence:
		//		 (translation *  rotation * scale * anchorTranslation) * Vertex
		// will be:
		//		rotation * scale.
		//
		// Therefore, anchor will be multiplied to existing matrix's  the right, and translation will be on the left.
		Mat4f transMatrix;
		transMatrix.SetTranslate(x, y, z);
		_transform = transMatrix * _transform;

		// TODO: anchor variables need changing
		_transform.Translate(-(float)(_rect.width)*_anchorRatio.x, -(float)(_rect.height)*_anchorRatio.y, 0.0f);
		
	}

	virtual const Vec3f& GetPosition() const{
		return _position;
	}

	virtual void SetScale(float x, float y){
		_scale.Set(x, y);

		// rest the transformation matrix to identity matrix
		_transform = Mat4f();

		// (translation *  rotation * scale * anchorTranslation) * Vertex
		_transform.Translate(_position.x, _position.y, _position.z);
		_transform.RotateZ(_rotation);
		_transform.Scale(x, y, 1.0f);
		// TODO: anchor variables need changing
		_transform.Translate(-(float)(_rect.width)*_anchorRatio.x, -(float)(_rect.height)*_anchorRatio.y, 0.0f);
	}

	virtual const Vec2f& GetScale() const{
		return _scale;
	}

	/**
	 *	 Get the anchor position related to the whole texture
	 */
	virtual Vec2f GetAnchor() const{
		return Vec2f(GetWidth()*_anchorRatio.x, GetHeight()*_anchorRatio.y);
	}

	/**
	 *	Set the width of the image. This actually update the scale x value of the image.
	 */
	virtual void SetWidth(float $w){
		SetScale($w/(float)_rect.width, _scale.y);
	}

	/**
	 *	 Set the height of the image. This actually update the scale y value of the image.
	 */
	virtual void SetHeight(float $h){
		SetScale(_scale.x, $h/(float)_rect.height);
	}

	/**
	 *	Set the width and height of the image. This actually update the scale values of the image.
	 */
	virtual void SetSize(float $w, float $h){
		SetScale($w/(float)_rect.width, $h/(float)_rect.height);
	}

	/**
	 *	 Get the width of the image. The width is the original width applied scale x.
	 */
	virtual const float GetWidth() const{
		return (float)(_rect.width)*_scale.x;
	}

	/**
	 *	 Get the height of the image. The height is the original height applied scale y.
	 */
	virtual const float GetHeight() const{
		return (float)(_rect.height)*_scale.y;
	}

	/**
	 *	Note that anchor translation will still be applied before this transformation!
	 */
	virtual void SetTransform(const Mat4f& matrix){
		_transform = matrix;

		// apply anchor translation first.
		Vec2f anchorTrans = GetAnchor();
		_transform.Translate(-anchorTrans.x, -anchorTrans.y, 0.0f);

		/**
		 *	We assume the matrix will only contains 2D affine transformation, and only an extra Z translation, for now, ignore the translation elements
		 *		| cos(r)*scaleX	-sin(r)*skewX		0	0  |
		 *		| sin(r)*skewY	cos(r)*scaleY		0	0  |
		 *		| 0			0				1	0  |
		 *		| 0			0				0	1  |
		 *
		 *	If we apply this matrix to a point at (1.0f, 0.0f, 0.0f)
		 *		x' = cos(r) * scaleX 
		 *		y' = sin(r) * skewY
		 *	
		 *	The rotation then will be:
		 *		atan2f(x', y')
		 *	===>>
		 *		_rotation = atan2f(_transform[1], _transform[0])
		 *
		 *
		 *	Also the scale will be:
		 *		scaleX = _transform[0] / cos(r)
		 *		scaleY = _transform[5] / cos(r)
		 */
		_rotation = atan2f(_transform[1], _transform[0]);

		float c = cosf(_rotation);
		_scale.Set(_transform[0]/c, _transform[5]/c);

		_position.Set(_transform[12], _transform[13], _transform[14]);
	}

	/**
	 *	Get the transformation matrix
	 */
	virtual const Mat4f& GetTransform() const{
		return _transform;
	}

	virtual void SetRect(const Recti& $rect){
		// copy assignment
		_rect = $rect;

		_vertices[0].v.Set(_rect.x, _rect.y, 0.0f);
		_vertices[1].v.Set(_rect.x + _rect.width, _rect.y, 0.0f);
		_vertices[2].v.Set(_rect.x + _rect.width, _rect.y + _rect.height, 0.0f);
		_vertices[3].v.Set(_rect.x, _rect.y + _rect.height, 0.0f);

		// calculate bottom left of the image in texture coordinate. 
		float u = (float)_rect.x/(float)_texture_sp->width();
		float v = (float)_rect.y/(float)_texture_sp->height();
		// Calculate the the width and height in texture coordinate.
		float w = (float)_rect.width/(float)_texture_sp->width();
		float h = (float)_rect.height/(float)_texture_sp->height();

		// assign the texture coordinate
		_vertices[0].uv.Set(u, v);
		_vertices[1].uv.Set(u + w, v);
		_vertices[2].uv.Set(u + w, v + h);
		_vertices[3].uv.Set(u, v + h);
	}

	virtual void SetRect(int $x, int $y, int $width, int $height){
		_rect.x = $x;
		_rect.y = $y;
		_rect.width = $width;
		_rect.height = $height;

		_vertices[0].v.Set(_rect.x, _rect.y, 0.0f);
		_vertices[1].v.Set(_rect.x + _rect.width, _rect.y, 0.0f);
		_vertices[2].v.Set(_rect.x + _rect.width, _rect.y + _rect.height, 0.0f);
		_vertices[3].v.Set(_rect.x, _rect.y + _rect.height, 0.0f);

		// calculate bottom left of the image in texture coordinate. 
		float u = (float)_rect.x/_texture_sp->width();
		float v = (float)_rect.y/_texture_sp->height();
		// Calculate the the width and height in texture coordinate.
		float w = (float)_rect.width/_texture_sp->width();
		float h = (float)_rect.height/_texture_sp->height();

		// assign the texture coordinate
		_vertices[0].uv.Set(u, v);
		_vertices[1].uv.Set(u + w, v);
		_vertices[2].uv.Set(u + w, v + h);
		_vertices[3].uv.Set(u, v + h);
	}

	virtual const Recti& GetRect() const{
		return _rect;
	}


	bool horizontalFlip;

protected:
	/**
	*	TODO: make sure copy constructor is doing things correct with the shared pointer.
	*	I assume it will simply do a copy with the pointer, and when the cloned image destructor is called, which will NULL the cloned pointer
	*	And try to remove the texture from the TextureManager. So it should be fine to use machine generated copy constructor.
	 */
	std::tr1::shared_ptr<ATexture> _texture_sp;

	/**
	 *	This define the part of texture this texture node is using. 
	 *	The actual offset position, width and height in texture resolution.
	 *	The partly texture will be mapped to the area defined by vertices.
	 *
	 *
	 */
	Recti _rect;

	/**
	 *	Create as many as vertices you want... Probably I'll create 4 for quad...	for now.
	 */
	Vertex3f* _vertices;

	/**
	 *	Showing how many vertices this node has.
	 */
	unsigned short _numVertices;

	/**
	 *	This matrix only contains 2D affine transformation. The reason I use Mat4f here is because I can easily use glLoadMatrix or glMultMatrix to contacted this transform matrix to the existing
	 *	OpenGL model view matrix. 
	 *
	 *	If we want to do render batching, every vertices of this IGraphics object will be manually applied to this transform matrix. It can be confusing since this is only a 2D render engine should not use 4x4 matrix.
	 *	But for future development, I decide to use consistent 4x4 matrix and x, y, z through out of the program. The extra dimension can be simply ignored.
	 *
	 *	But it does require extra memory to store 4x4 matrix. Probably slower...
	 *
	 *	Note that we also need to decomposing the transform matrix into: rotation, scale, translation. So the state of the graphics object can be easily identified. This should be relatively easy since we only consider 2D affine transformation.
	 *	Every time a transform matrix is directly set through setter. The decomposing process will be performed to sync the scale, rotation and translation variables. Detail to see the SetTransform() method.
	 */
	Mat4f _transform;

	/**
	 *	scale of the image
	 */
	Vec2f _scale;

	/**
	 *	
	 */
	Vec3f _position;

	/**
	 *	
	 */
	float _rotation;

	/**
	 *	Set anchor ratio internal use only
	 */
	Vec2f _anchorRatio;

	/**
	 *	The index array to tell OpenGL how to use the vertices. E.g., we have 0, 1, 2, 3 vertices.
	 *	If we want to draw 2 triangle to form a quad, we should assign this to _indices array:
	 *		[0, 1, 2,  2, 1, 3] ?????
	 */
	GLubyte* _indices;
};

