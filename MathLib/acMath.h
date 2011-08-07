/** @page license License
 * vmath, set of classes for computer graphics mathematics.
 * Copyright (c) 2005-2011, Jan Bartipan < barzto at gmail dot com >
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met:
 *
 * - Redistributions of source code must retain the above copyright 
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright 
 *   notice, this list of conditions and the following disclaimer in 
 *   the documentation and/or other materials provided with the 
 *   distribution.
 * - Neither the names of its contributors may be used to endorse or 
 *   promote products derived from this software without specific 
 *   prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
* Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

// Simplified from vmath and Box2D
#pragma once
#include <cmath>
#include <limits>
#include <sstream>

#define ac_pi 3.1415926

//################################################################
//######################## General functions ##########################
//################################################################
inline double unifRand(){
	return rand()/double(RAND_MAX);
}

inline bool isValid(float v){
	// NaN, not a number. Only NaN not equal to itself.
	if(v != v)
		return false;
	float infinity = std::numeric_limits<float>::infinity();
	// check to see whether it exceeds the limit.
	return -infinity < v && v < infinity;
}

inline unsigned int nextPowerOfTwo(unsigned int v){
	v |= (v >> 1);
	v |= (v >> 2);
	v |= (v >> 4);
	v |= (v >> 8);
	v |= (v >> 16);
	return v+1;
}

inline bool isPowerOfTwo(unsigned int v){
	return (v > 0) && (v & (v - 1)) == 0;
}

/// absolute
template<typename T>
inline float acAbs(T a){
	return a > 0.0f ? a : -a;
}

/// give the min value
template <typename T>
inline T acMin(T a, T b){
	return a < b ? a : b;
}

/// give the max value
template <typename T> 
inline T acMax(T a, T b){
	return a > b ? a : b;
}

/// swap shortcut
template <typename T> 
inline void swap(T& a, T& b){
	T temp = a;
	a = b;
	b = temp;
}

//################################################################
//################ Vector2 related class and function ###################### 
//################################################################
template<class T> 
class Vec2
{
public:
	union{
		// Normal x coordinate
		T x;

		/// Texture coordinate.
		T u;
	};

	union{
		T y;

		T v;
	};

	// =================== constructors =======================

	Vec2(void){}

	Vec2(T ix, T iy):x(ix), y(iy){}

	/// Copy constructor.
	Vec2(const Vec2<T>& v): x(v.x), y(v.y){}

	/// casting constructor
	template<class NT>
	Vec2(const Vec2<NT>& v): x(static_cast<T>(v.x)), y(static_cast<T>(v.y)){} 

	/// ===================== access ========================
	/// Copy the right hand side vector to left and do the casting
	template<class NT>
	Vec2<T>& operator= (const Vec2<NT>& v){
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);

		return *this;
	}

	/// Normal copy operator, copy right hand side vector.
	Vec2<T>& operator= (const Vec2<T>& v){
		x = v.x;
		y = v.y;

		return *this;
	}

	/// Shortcut to zero
	void SetZero(){
		x = y = 0;
	}

	/// Shortcut setter
	void Set(T ix, T iy){
		x = ix;
		y = iy;
	}

	/// read the indexed component
	T operator[] (int i) const{
		return (&x)[i];
	}

	/// write to the indexed component.
	T& operator[] (int i){
		return (&x)[i];
	}

	//========================= arithmetic ===================
	/// reverse of the vector
	Vec2<T> operator- () const{
		Vec2 v;
		v.Set(-x, -y);

		return v;
	}

	/// Vector addition
	Vec2<T> operator+ (const Vec2<T>& v) const{
		return Vec2<T>(x + v.x, y + v.y);
	}

	/// addition
	void operator+= (const Vec2<T>& v){
		x += v.x;
		y += v.y;
	}

	// Vector subtraction
	Vec2<T> operator- (const Vec2<T>& v) const{
		return Vec2<T>(x - v.x, y - v.y);
	}

	/// subtraction
	void operator-= (const Vec2<T>& v){
		x -= v.x;
		y -= v.y;
	}

	/// scale, scalar is on the right of the vector.
	Vec2<T> operator* (const T s){
		return Vec2<T>(x*s, y*s);
	}

	/// scale
	void operator*= (const T s){
		x *= s;
		y *= s;
	}

	/// Dot product
	T Dot(const Vec2<T>& v) const{
		return x*v.x + y*v.y;
	}

	/// Scalar cross product. There are two inline 2d pseudo  cross products as well, which produce a new vector perpendicular to this vector.
	T Cross(const Vec2<T>& v) const{
		return x*v.x - y*v.y;
	}

	Vec2<T> Abs(const Vec2<T>& v){
		return Vec2<T>(acAbs(v.x), acAbs(v.y));
	}

	// ========================  comparison ================================
	/// Comparison, compare the pointer pointed content values.
	bool operator== (const Vec2& v){
		return x == v.x && y == v.y;
	}

	/// Comparison, compare the pointer pointed content values.
	bool operator!= (const Vec2& v){
		return !(*this == v);
	}

	// =========================== size setter and query =============================
	T Len() const{
		return (T)std::sqrt(x*x + y*y);
	}

	T Len2() const{
		return x*x + y*y;
	}

	void Normalize(){
		T invLen = 1/this->Len();
		x *= invLen;
		y *= invLen;
	}

	// ==================== Conversion to pointer operator,  From vmath library. ==============================

	// For passing Vec3<T> values to gl*3[fd] functions.
	operator T* (){
		return (T*)this;
	}

	// For passing Vec3<T> values to gl*3[fd] functions.
	operator const T* () const{
		return (const T*)this;
	}


	
	// ============================= print out functions ==============================
	// FIXME:  IntelliSense: more than one operator "<<" matches these operands
	friend std::ostream& operator<< (std::ostream& stream, const Vec2<T> v){
		stream << "[" << v.x << ", " << v.y << "]";
		return stream;
	}

	std::string ToString() const{
		std::ostringstream s;
		s << *this;
		return s.str();
	}
};

// ================ extra arithmetic functions for Vec2 =======================
/// Vector scale. Scalar is on the left of the vector.
template<class T>
inline Vec2<T> operator* (T s, const Vec2<T>& v)
{
	return Vec2<T>(s * v.x, s * v.y);
}

/// 2d pseudo  cross product , create a perpendicular vector which lays on the RIGHT of the vector.
template<class T>
inline Vec2<T> Cross(const Vec2<T>& v, float s){
	return Vec2<T>(s*v.y, -s*v.x);
}

/// 2d pseudo  cross product, create a perpendicular vector which lays on the LEFT of the vector.
template<class T>
inline Vec2<T> Cross(float s, const Vec2<T>& v){
	return Vec2<T>(-s*v.y, s*v.x);
}


// ===================== Vec2 util functions ==============================
template <class T>
inline Vec2<T> acMin(const Vec2<T>& v1, const Vec2<T>& v2){
	return Vec2<T>(acMin(v1.x, v2.x), acMin(v1.y, v2.y));
}

template <class T> 
inline Vec2<T> acMax(const Vec2<T>& v1, const Vec2<T>& v2){
	return Vec2<T>(acMax(v1.x, v2.x), acMax(v1.y, v2.y));
}

template<class T> 
inline Vec2<T> acAbs(const Vec2<T>& v){
	return Vec2<T>(acAbs(v.x), acAbs(v.y));
}


//####################################################################################
//################			vector3 related class and functions.                            ####################
//####################################################################################
template<class T>
class Vec3 {
public:
	union{
		// normal x coordinate
		T x;

		// Texture u coordinate
		T u;

		// color red
		T r;
	};

	union{
		// normal y coordinate
		T y;

		// Texture v coordinate
		T v;

		// color green
		T g;
	};

	union{
		// normal y coordinate
		T z;

		// texture w coordinate
		T w;

		// color blue
		T b;
	};

	Vec3(void) {}

	Vec3(T $x, T $y, T $z): x($x), y($y), z($z) {}

	// copy constructor
	Vec3(const Vec3<T>& v): x(v.x), y(v.y), z(v.z) {}

	// casting constructor.
	template<class NT>
	Vec3(const Vec3<NT>& v): x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast(v.z)) {}

	// ==================================== access functions ===================================

	// Directly copy the right hand side vector3 into left and do the casting
	template<class NT>
	Vec3<T>& operator= (const Vec3<NT>& v){
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
		z = static_cast<T>(v.z);

		return *this;
	}

	// Normal copy assignment operator
	Vec3<T>& operator= (const Vec3<T>& v){
		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}

	// Zero the vector
	void SetZero(){
		x = y = z = 0;
	}

	// Shortcut setter
	void Set(T $x, T $y, T $z){
		x = $x;
		y = $y;
		z = $z;
	}

	// Read the indexed component
	T operator[] (int i) const{
		return (&x)[i];
	}

	// Write to the indexed component
	T& operator[] (int i){
		return (&x)[i];
	}

	//========================= arithmetic ===================

	// Reverse of the vector
	Vec3<T> operator- () const{
		Vec3 v;
		v.Set(-x, -y, -z);

		return v;
	}

	// Addition
	Vec3<T> operator+ (const Vec3<T>& v){
		return Vec3<T>(x+v.x, y+v.y, z+v.z);
	}

	// Add to itself
	void operator+= (const Vec3<T>& v){
		x += v.x;
		y += v.y;
		z += v.z;
	}

	// Subtraction
	Vec3<T> operator- (const Vec3<T>& v){
		return Vec3<T>(x-v.x, y-v.y, z-v.z);
	}

	// Subtract from itself
	void operator-= (const Vec3<T>& v){
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}

	// Scale vector
	Vec3<T> operator* (const T s){
		return Vec3<T>(x*s, y*s, z*s);
	}

	// Scale itself
	void operator*= (const T s){
		x *= s;
		y *= s;
		z *= s;
	}

	// Dot product
	T Dot(const Vec3<T>& v) const{
		return x*v.x + y*v.y + z*v.z;
	}

	// Cross product, result a new vector perpendicular to the two vectors. 
	// The result vector's direction can be specified by right-hand rule.
	//	1. Index finger pointed to "this" vector's direction
	//	2. Middle finger pointed to the input vector direction.
	//	3. Result vector is directed by thumb perpendicular to the two fingers.
	Vec3<T> Cross(const Vec3<T>& v){
		return Vec3<T>(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
	}


	// ========================= Comparison =================================

	/// Comparison, compare the pointer pointed content values.
	bool operator== (const Vec3& v){
		return x == v.x && y == v.y && z == v.z;
	}

	/// Comparison, compare the pointer pointed content values.
	bool operator!= (const Vec3& v){
		// call the overloaded == operator, if they are equal, then they are not equal.....
		return !(*this == v);
	}

	// =========================== size setter and query =============================
	T Len() const{
		return (T)std::sqrt(x*x + y*y + z*z);
	}

	T Len2() const{
		return x*x + y*y + z*z;
	}

	void Normalize(){
		T invLen = 1/this->Len();
		x *= invLen;
		y *= invLen;
		z *= invLen;
	}


	// ==================== Conversion to pointer operator,  From vmath library. ==============================

	// For passing Vec3<T> values to gl*3[fd] functions.
	operator T* (){
		return (T*)this;
	}

	// For passing Vec3<T> values to gl*3[fd] functions.
	operator const T* () const{
		return (const T*)this;
	}


	// ============================= print out functions ==============================
	// FIXME:  IntelliSense: more than one operator "<<" matches these operands
	friend std::ostream& operator<<(std::ostream& stream, const Vec3<T> v){
		stream << "[" << v.x << ", " << v.y << ", " << v.z << "]";
		return stream;
	}

	std::string ToString() const{
		std::ostringstream s;
		s << *this;
		return s.str();
	}
};

// ================ extra arithmetic functions for Vec2 =======================
/// Vector scale. Scalar is on the left of the vector.
template<class T>
inline Vec3<T> operator* (T s, const Vec3<T>& v)
{
	return Vec3<T>(s*v.x, s*v.y£¬ s*v.z);
}

// ===================== Vec3 util function ==============================
template <class T>
inline Vec3<T> acMin(const Vec3<T>& v1, const Vec3<T>& v2){
	return Vec3<T>(acMin(v1.x, v2.x), acMin(v1.y, v2.y), acMin(v1.z, v2.z));
}

template <class T> 
inline Vec3<T> acMax(const Vec3<T>& v1, const Vec3<T>& v2){
	return Vec2<T>(acMax(v1.x, v2.x), acMax(v1.y, v2.y), acMax(v1.z, v2.z));
}

template<class T> 
inline Vec3<T> acAbs(const Vec3<T>& v){
	return Vec3<T>(acAbs(v.x), acAbs(v.y), acAbs(v.z));
}


template<class T>
class Vec4 {
public:
	union{
		// x coordinate
		T x;

		// red
		T r;
	};

	union{
		// y coordinate
		T y;

		// green
		T g;
	};

	union{
		// z coordinate
		T z;

		// blue
		T b;
	};

	union{
		//  w coordinate, for homogeneous coordinate.
		T w;

		// alpha
		T a;
	};

	Vec4(void) {}

	Vec4(T $x, T $y, T $z, T $w): x($x), y($y), z($z), w($w){}

	// copy constructor
	Vec4(const Vec4<T>& v): x(v.x), y(v.y), z(v.z), w(v.z) {}

	// casting constructor.
	template<class NT>
	Vec4(const Vec4<NT>& v): x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast(v.z)), w(static_cast(v.w)) {}


	// ==================================== access functions ===================================

	// Directly copy the right hand side vector4 into left and do the casting
	template<class NT>
	Vec4<T>& operator= (const Vec4<NT>& v){
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
		z = static_cast<T>(v.z);
		w = static_cast<T>(v.z);

		return *this;
	}

	// Normal copy assignment operator
	Vec4<T>& operator= (const Vec4<T>& v){
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;

		return *this;
	}

	// Zero the vector
	void SetZero(){
		x = y = z = w = 0;
	}

	// Shortcut setter
	void Set(T $x, T $y, T $z, T $w){
		x = $x;
		y = $y;
		z = $z;
		w = $w;
	}

	// Read the indexed component
	T operator[] (int i) const{
		return (&x)[i];
	}

	// Write to the indexed component
	T& operator[] (int i){
		return (&x)[i];
	}
};




//####################################################################################
//################ 2*2 Matrix related class and function. For affine transformation ####################
//####################################################################################
template<class T>
class Mat2{
public:
	Mat2(){}

	/// Construct this matrix using columns.
	Mat2(const Vec2<T>& c1, const Vec2<T>& c2): col1(c1), col2(c2){}

	// |  a11    a21   |
	// |  a12    a22  |
	Mat2(T a11, T a12, T a21, T a22){
		col1.x = a11; col1.y = a21;
		col2.x = a12; col2.y = a22;
	}

	// Construct a affine rotation matrix.
	// Use explicit to avoid undesirable conversion.
	// Otherwise you will be able to do this:
	//	Mat2 m = 888;
	// which does not make any sense in the first glance. 
	// Just following the google c++ style.
	explicit Mat2(T radian){
		T c = cos(radian);
		T s = sin(radian);
		col1.x = c;  col2.x = -s;
		col1.y = s;  col2.y = c;
	}

	void Set(const Vec2<T>& c1, const Vec2<T>& c2){
		col1 = c1;
		col2 = c2;
	}

	void Set(T radian){
		T c = cos(radian);
		T s = sin(radian);
		col1.x = c;  col2.x = -s;
		col1.y = s;  col2.y = c;
	}

	void SetIdentity(){
		col1.x = 1;  col2.x = 0;
		col1.y = 0;  col2.y = 1;
	}

	void SetZero(){
		col1.SetZero();
		col2.SetZero();
	}

	T GetRadian(){
		return atan2(col1.y, col1.x);
	}

	Vec2<T> Multiply(const Vec2<T>& v) const{
		return Vec2<T>(col1.x*v.x + col2.x*v.y,  col1.y*v.x + col2.y*v.y);
	}

	Vec2<T> col1, col2;
};


/**
 *	A column major 4x4 matrix. You can pass the matrix directly to OpenGL
 *		| m0   m4   m8     m12  |
 *		| m1   m5   m9     m13  |
 *		| m2   m6   m10   m14  |
 *		| m3   m7   m11   m15  |          
 */
template<class T>
class Mat4{
public:
	/**	
	 *	| m0   m4   m8     m12  |
	 *	| m1   m5   m9     m13  |
	 *	| m2   m6   m10   m14  |
	 *	| m3   m7   m11   m15  |                 
	 *
	 *	v' = M * v
	 *		===>>>
	 *	x¡¯	=	 x*m0   +   y*m4   +   z*m8      +   m12
	 *	y¡¯	=	 x*m1   +   y*m5   +   z*m9      +   m13
	 *	z¡¯	=	 x*m2   +   y*m6   +   z*m10    +   m14
	 *	w¡¯	=	 x*m3   +   y*m7   +   z*m11    +   m15
	 */
	T m[16];

	/*
	 * by default, this is a identity matrix 
	 *
	 *	| 1  0  0  0  |
	 *	| 0  1  0  0  |
	 *	| 0  0  1  0  |
	 *	| 0  0  0  1  |     
	 */
	Mat4(){
		for(int i=0; i<16; ++i){
			m[i] = i%5 ? 0 : 1;
		}
	}

	// Copy constructor
	Mat4(const Mat4<T>& mat){
		std::memcpy(m, mat.m, sizeof(T)*16);
	}

	// Copy and casting constructor
	template<class NT>
	Mat4(const Mat4<NT>& mat){
		for(int i=0; i<16; ++i){
			m[i] = static_cast<T>(mat.m[i]);
		}
	}

	// pass in an array of T
	Mat4(const T* arr){
		std::memcpy(m, arr, sizeof(T)*16);
	}

	// ==================== access method ============================
	// Column row setter
	T& operator() (int columnIndex, int rowIndex){
		return m[columnIndex*4 + rowIndex];
	}

	// Column row getter
	const T& operator() (int columnIndex, int rowIndex) const{
		return m[columnIndex*4 + rowIndex];
	}

	/**
	 *	 Direct array setter, save some typing... might be slower than use public member "m"
	 */
	T& operator[] (int index){
		return m[index];
	}

	/**
	 *	Direct array getter, save some typing... might be slower than use public member "m"
	 */
	const T& operator[] (int index) const{
		return m[index];
	}

	// copy assignment
	Mat4<T>& operator= (const Mat4<T>& mat){
		std::memcpy(m, mat.m, sizeof(T)*16);
		return *this;
	}

	// ================== transform methods ===========================

	// Set this matrix to identity matrix
	void SetIdentity(){
		for(int i=0; i<16; ++i){
			m[i] = i%5 ? 0 : 1;
		}
	}

	/**	
	 *	Set this matrix to translation matrix.
	 *		| m0  m4  m8	 tx  |
	 *		| m1  m5  m9	 ty  |
	 *		| m2  m6  m10	 tz  |
	 *		| m3  m7  m11	1    | 
	 */
	void SetTranslate(const Vec3<T>& v){
		m[12] = v.x;
		m[13] = v.y;
		m[14] = v.z;
		m[15] = 1;
	}

	/**
	 *	Get the translation of the matrix.
	 */
	Vec3<T> GetTranslate(){
		return Vec3<T>(m[12], m[13], m[14]);
	}

	/**
	 *	Set this matrix to scale matrix:
	 *		| x	0	0	0  |
	 *		| 0	y	0	0  |
	 *		| 0	0	z	0  |
	 *		| 0	0	0	1  |     
	 *
	 */
	void SetScale(float x, float y, float z){
		m[0] = x;
		m[5] = y;
		m[10] = z;
	}

	/**
	 *	Set this matrix to Z rotation matrix.	
	 *
	 *	The final rotation r' is original rotation add the new rotation
	 *	r' = or + nr
	 *
	 *	Therefore a vector "v" rotated by r' degree, result "v'" will be: 
	 *		v'.x = v.length * cos( r' )		===>>	v'.x = v.length * cos( or + nr )
	 *		v'.y = v.length * sin( r' )		===>>	v'.y = v.length * sin( or + nr )
	 *
	 *	let: 
	 *		s = v.length
	 *	Then we have:
	 *		v'.x = s * cos(or) * cos(nr)  - s * sin(or) * sin(nr)
	 *		v'.y = s * sin(or) * cos(nr) + s * cos(or) * sin(nr)
	 *
	 *	Cause the original vector v's position is:
	 *		v.x = s * cos(or)
	 *		v.y = s * sin(or)
	 *
	 *	Combine 4 equations above gives:
	 *		v'.x = v.x  * cos(nr) - v.y * sin(nr)
	 *		v'.y = v.y * cos(nr) + v.x * sin(nr)	==>>	 v'.y = v.x * sin(nr) + v.y * cos(nr)
	 *
	 *
	 *	If we have OpenGL style column major matrix, M:
	 *		| a	c |	
	 *		| b	d |	
	 *	and vector, V:
	 *		| v.x |
	 *		| v.y |
	 *
	 *	V' = M * V will be:
	 *		| a * v.x + c * v.y |
	 *		| b * v.x + d * v.y |
	 *
	 *
	 *	Because:
	 *		v'.x = v.x  * cos(nr) - v.y * sin(nr)
	 *		v'.y = v.x * sin(nr) + v.y * cos(nr)
	 *
	 *	We can say that:
	 *		a = cos(nr)		c = -sin(nr)
	 *		b = sin(nr)		d = cos(nr)
	 *	===>
	 *		| cos(nr)	-sin(nr) |
	 *		| sin(nr)	 cos(nr) |
	 *
	 *
	 *	| cos(r)	-sin(r)		0	0  |
	 *	| sin(r)	cos(r)		0	0  |
	 *	| 0		0		1	0  |
	 *	| 0		0		0	1  |
	 *	
	 */
	void SetRotateZ(T radian){
		// a cosine
		float ac = cos(radian);
		// b sine
		float bs = sin(radian);
		// c -sine
		float cs = -sin(radian);
		// d cosine
		float dc = cos(radian);

		m[0] = ac;
		m[1] = bs;
		m[4] = cs;
		m[5] = dc;
	}

	// TODO: do other proper rotation operation.

	//========================= arithmetic ===================

	/**
	 *	 matrix concatenation,  multiplication
	 *	
	 *	currentMatrix * newMatrix
	 *							
	 *	| c0   c4   c8     c12  |			| m0   m4   m8     m12  |
	 *	| c1   c5   c9     c13  |		* 	| m1   m5   m9     m13  |
	 *	| c2   c6   c10   c14  |			| m2   m6   m10   m14  |
	 *	| c3   c7   c11   c15  |			| m3   m7   m11   m15  |
	 *
	 *			====>>>>
	 *	
	 *	n0 = c0 * m0	+	c4 * m1	+	c8 * m2	+	c12 * m3
	 *	n1 = c1 * m0	+	c5 * m1	+	c9 * m2	+	c13 * m3
	 *	n2 = c2 * m0	+	c6 * m1	+	c10*m2	+	c14 * m3
	 *	n3 = c3 * m0	+	c7 * m1	+	c11*m2	+	c15 * m3
	 *	
	 *	n4 = c0 * m4	+	c4 * m5	+	c8 * m6	+	c12 * m7
	 *					..............
	 *
	 *	| n0   n4   n8     n12  |
	 *	| n1   n5   n9     n13  |
	 *	| n2   n6   n10   n14  |
	 *	| n3   n7   n11   n15  |
	 */
	Mat4<T> operator* (const Mat4<T>& mat){
		// The new matrix
		Mat4<T> nMat;
		// Since the OpenGL matrix is column major.
		// We have to scan and assign the matrix element in VERTICAL order.

		// Column iteration
		for(int x=0; x<4; ++x){
			// Row iteration
			for(int y=0; y<4; ++y){
				// Store the two tuples multiplication result.
				T n=0;
				
				// Do the ROW * COLUMN
				// 
				// e.g., for row 0, column 0
				//	c0*m0  +  c4*m1  +  c8*m2   +  c12*m3
				for(int k=0; k<4; ++k){
					// take the current matrix's ROW multiply to the input matrix's COLUMN
					n += this(k, y) * mat(x, k);
				}

				// assign the multiplication result to current matrix element
				nMat(x, y) = n;
			}
		}
	}

	/**
	 * Matrix  multiply a vector3. 
	 * This means apply transformation to the vector3. Note that w' is ditched.
	 *	v' = M * v
	 *		===>>>
	 *	x¡¯	=	 x*m0   +   y*m4   +   z*m8      +   m12
	 *	y¡¯	=	 x*m1   +   y*m5   +   z*m9      +   m13
	 *	z¡¯	=	 x*m2   +   y*m6   +   z*m10    +   m14
	 *
	 *	w¡¯	=	 x*m3   +   y*m7   +   z*m11    +   m15
	 */
	Vec3<T> operator* (const Vec3<T>& v){
		return Vec3<T>(
			v.x*m[0] + v.y*m[4] + v.z*m[8]  + m[12],
			v.x*m[1] + v.y*m[5] + v.z*m[9]  + m[13],
			v.x*m[2] + v.y*m[6] + v.z*m[10] + m[14]
			);
	}

	/**
	 *	Transform the Vector4 with this matrix.
	 */
	Vec4<T> operator* (const Vec4<T>& v){
		return Vec4<T>(
			v.x*m[0] + v.y*m[4] + v.z*m[8]  + v.w*m[12],
			v.x*m[1] + v.y*m[5] + v.z*m[9]  + v.w*m[13],
			v.x*m[2] + v.y*m[6] + v.z*m[10] + v.w*m[14],
			v.x*m[3] + v.y*m[7] + v.z*m[11] + v.w*m[15]
			);
	}

	//================ For OpenGL ======================
	operator T*(){
		return (T*)m;
	}

	operator T*() const{
		return (T*)m;
	}
};

//#######################################################################
//###################### Transform class and functions ##########################
//#######################################################################
template<class T>
class Transform2{
public:
	Transform2(){}

	Transform2(const Vec2<T>& t, const Mat2<T>& r): translation(t), rotation(r){}

	void SetIdentity(){
		translation.SetZero();
		rotation.SetIdentity();
	}

	void Set(const Vec2<T>& t, T radian)
	{
		translation = t;
		rotation.Set(radian);
	}

	T GetRadian() const{
		return atan2(rotation.col1.y, rotation.col1.x);
	}

	Vec2<T> Multiply(const Vec2<T>& v) const
	{
		T x = translation.x + rotation.col1.x * v.x + rotation.col2.x * v.y;
		T y = translation.y + rotation.col1.y * v.x + rotation.col2.y * v.y;

		return Vec2<T>(x, y);
	}

	/// translation vector
	Vec2<T> translation;
	/// rotation matrix
	Mat2<T> rotation;
};


//################################################################
//###################### Class type shortcut ###########################
//################################################################
typedef class Vec2<float> Vec2f;
typedef class Vec2<double> Vec2d;
typedef class Vec2<int> Vec2i;
typedef class Vec2<short> Vec2s;

typedef class Vec3<float> Vec3f;
typedef class Vec3<double> Vec3d;
typedef class Vec3<int> Vec3i;
typedef class Vec3<short> Vec3s;

typedef class Vec4<float> Vec4f;
typedef class Vec4<double> Vec4d;
typedef class Vec4<int> Vec4i;
typedef class Vec4<short> Vec4s;


typedef class Mat2<float> Mat2f;
typedef class Mat2<double> Mat2d;
typedef class Mat2<int> Mat2i;

typedef class Mat4<float> Mat4f;
typedef class Mat4<double> Mat4d;
typedef class Mat4<int> Mat4i;

typedef class Transform2<float> Transform2f;
typedef class Transform2<double> Transform2d;
typedef class Transform2<int> Transform2i;


//#################################################################
//######################         Constants          ###########################
//################################################################
const Vec2f vec2_zero(0.0f, 0.0f);
const Mat2f mat2_identity(1.0f, 0.0f, 0.0f, 1.0f);
const Transform2f transform2_identity(vec2_zero, mat2_identity);



//#####################################################################################
//################################ thing to modify  ########################################
//#####################################################################################
template<typename T>
struct Rect{
	T x, y, width, height;

	Rect(): x(0), y(0), width(0), height(0){}

	Rect(T $x, T $y, T $w, T $h): x($x), y($y), width($w), height($h){}

	void Set(T $x, T $y, T $w, T $h){
		x = $x;
		y = $y;
		width = $w;
		height = $h;
	}

	T Top(){
		return y + height;
	}
	T Right(){
		return x + width;
	}
	T Bottom(){
		return y;
	}
	T Left(){
		return x;
	}

	Vec2<T> TopLeft(){
		Vec2<T> p;
		p.x = x;
		p.y = y + height;

		return p;
	}

	Vec2<T> TopRight(){
		Vec2<T> p;
		p.x = x + width;
		p.y = y + height;

		return p;
	}

	Vec2<T> BottomLeft(){
		Vec2<T> p;
		p.x = x;
		p.y = y;

		return p;
	}

	Vec2<T> BottomRight(){
		Vec2<T> p;
		p.x = x + width;
		p.y = y;

		return p;
	}

	Rect<T> Clone(){
		return Rect<T>(x, y, width, height);
	}

	void Shrink(T v){
		x += v;
		y += v;
		width -= v*2;
		height -= v*2;
	}
};

typedef struct Rect<float> Rectf;
typedef struct Rect<double> Rectd;
typedef struct Rect<int> Recti;