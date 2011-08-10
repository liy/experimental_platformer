#pragma once
#include "acMath.h"

/**
 *	The vertex class, which have corresponding total 8 bytes texture coordinate,  the total 4 bytes colour and depending on the T type there are 3 components: 3 * sizeof(T)
 *	( 1 byte for unsigned char, 4 bytes for float)
 *
 *
 */
// FIXME: in order to use the offsetof() function I use struct here. Probably should not use both offsetof() and struct extends class?
template<class T>
struct Vertex3{
	/**
	 *	Texture coordinate, uv coordinate. Total 8 bytes
	 */
	Vec2f uv;

	/**
	 *	
	 */
	Vec3<T> v;

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
 *	The vertex contains:
		1. A 3d vector, 12 bytes
		2. A 2d floating vector texture coordinate, 8 bytes
		3. A 4d vector for RGBA colour of the vertex. 4 bytes
 *	Total 24 bytes
 */
typedef class Vertex3<float> Vertex3f;
/**
 *	
 */
typedef class Vertex3<double> Vertex3d;