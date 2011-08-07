#pragma once
#include "acMath.h"

/**
 *	The vertex class, which have corresponding total 8 bytes texture coordinate,  the total 4 bytes colour and depending on the T type there are 3 components: 3 * sizeof(T)
 *	( 1 byte for unsigned char, 4 bytes for float)
 */
template<class T>
class Vertex3 : public Vec3<T>{
	/**
	 *	Texture coordinate. Total 8 bytes
	 */
	Vec2f texCoord;

	/**
	 *	The colour of the vertex. Total 4 bytes.
	 */
	Vec4ub colour;
};

/**
 *	
 */
typedef class Vertex3<int> Vertex3i;
/**
 *	Total 24 bytes
 */
typedef class Vertex3<float> Vertex3f;
/**
 *	
 */
typedef class Vertex3<double> Vertex3d;