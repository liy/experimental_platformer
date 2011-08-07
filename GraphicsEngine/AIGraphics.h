#pragma once
#include "AGeom.h"
#include <iostream>

class AIGraphics
{
public:
	AIGraphics(void){
		// init tint colour
		colour.Set(1.0f, 1.0f, 1.0f, 1.0f);

		// setup default scale and anchor position
		scale.Set(1.0f, 1.0f);
		anchorRatio.Set(0.5f, 0.5f);
		horizontalFlip = false;

		_transform = mat4_identity;
	};

	virtual ~AIGraphics(void){};

	// scale of the image
	Vec2f scale;

	// tint colour of the image
	Vec4f colour;

	// Set anchor ratio internal use only
	Vec2f anchorRatio;

	bool horizontalFlip;

	// Update the graphic, this probably only apply to the animation. For static image, this function does nothing
	virtual void Update(unsigned short delta) {};

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

	// Get the anchor position related to the whole texture
	virtual Vec2f anchor() const =0;

	// Set the width of the image. This actually update the scale x value of the image.
	virtual void setWidth(float $w) =0;

	// Set the height of the image. This actually update the scale y value of the image.
	virtual void setHeight(float $h) =0;

	// Set the width and height of the image. This actually update the scale values of the image.
	virtual void setSize(float $w, float $h) =0;

	// Get the width of the image. The width is the original width applied scale x.
	virtual const float width() const =0;

	// Get the height of the image. The height is the original height applied scale y.
	virtual const float height() const =0;

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
		_transform.Scale(scale.x, scale.y, 1.0f);
		// TODO: anchor variables need changing
		Vec2f anchorTrans = anchor();
		_transform.Translate(anchorTrans.x, anchorTrans.y, 0.0f);
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

		Vec2f anchorTrans = anchor();
		_transform.Translate(anchorTrans.x, anchorTrans.y, 0.0f);
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
		_transform.Scale(scale.x, scale.y, 1.0f);
		// TODO: anchor variables need changing
		Vec2f anchorTrans = anchor();
		_transform.Translate(anchorTrans.x, anchorTrans.y, 0.0f);
	}

	virtual const Vec2f& GetScale() const{
		return _scale;
	}

	/**
	 *	Note that anchor translation will still be applied before this transformation!
	 */
	virtual void SetTransform(const Mat4f& matrix){
		_transform = matrix;

		// apply anchor translation first.
		Vec2f anchorTrans = anchor();
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
	virtual const Mat4f GetTransform() const{
		return _transform;
	}

protected:

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
	 *	
	 */
	Vec2f _scale;

	/**
	 *	
	 */
	Vec3f _position;

	float _rotation;
};

