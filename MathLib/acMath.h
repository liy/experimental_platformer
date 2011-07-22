#pragma once

#include <cmath>
#include <cfloat>
#include <cstddef>
#include <limits>
#include <stdlib.h>

#define a2_pi 3.1415926;

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
	T Dot(const Vec2<T> v) const{
		return x*v.x + y*v.y;
	}

	/// Scalar cross product. There are two inline 2d pseudo  cross products as well, which produce a new vector perpendicular to this vector.
	T Cross(const Vec2<T> v) const{
		return x*v.x - y*v.y;
	}

	Vec2<T> Abs(const Vec2<T>& v){
		return Vec2<T>(acAbs(v.x), acAbs(v.y));
	}

	// ========================  comparison ================================
	/// Comparison
	bool operator== (const Vec2& v){
		return x == v.x && y == v.y;
	}

	// =========================== size setter and query =============================
	T Length() const{
		return (T)std::sqrt(x*x + y*y);
	}

	T LengthSquared() const{
		return x*x + y*y;
	}

	void Normalize(){
		T inv_len = this->Length();
		x *= inv_len;
		y *= inv_len;
	}
};

//===================== pseudo cross product ==============================
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


// ===================== Vector2D util function ==============================
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


//#######################################################################
//###################### Transfrom class and functions ##########################
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
typedef class Mat2<float> Mat2f;
typedef class Mat2<double> Mat2d;
typedef class Mat2<int> Mat2i;
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