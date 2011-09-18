#include "ATexture.h"
#include <iostream>
#include "acMath.h"
#include <windows.h>
#include <GL\GL.h>

ATexture::ATexture(void): _textureID(0), _contentWidth(0), _contentHeight(0), _width(0), _height(0), _bpp(0)
{

}

ATexture::ATexture( unsigned int contentWidth, unsigned int contentHeight, GLenum inputFormat, unsigned char bpp/*=4*/ )
{
	Create(contentWidth, contentHeight, inputFormat, bpp);
}

ATexture::ATexture( GLubyte* buffer, unsigned int contentWidth, unsigned int contentHeight, unsigned int width, unsigned int height, GLenum pixelDataFormat, unsigned char bpp/*=4*/ )
{
	Create(buffer, contentWidth, contentHeight, width, height, pixelDataFormat, bpp);
}

ATexture::~ATexture(void)
{
	std::cout << "*** ATexture[" << _textureID <<"] destroyed ***\n";
	glDeleteTextures(1, &_textureID);
}

GLuint ATexture::Create( unsigned int contentWidth, unsigned int contentHeight, GLenum pixelDataFormat, unsigned char bpp/*=4*/ )
{
	_bpp = bpp;

	_width = _contentWidth = contentWidth;
	_height = _contentHeight = contentHeight;

	if(!isPowerOfTwo(_contentWidth))
		_width = nextPowerOfTwo(_contentWidth);
	if(!isPowerOfTwo(_contentHeight))
		_height = nextPowerOfTwo(_contentHeight);

	// create a data buffer for drawing the web view
	GLubyte* buffer = new GLubyte[_width * _height * _bpp];

	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);

	// Select modulate to make tint effect when supply color information
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// Setup the scale up scale down interpolation method.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Setup the wrapping method.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	if(_bpp == 4){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, pixelDataFormat, GL_UNSIGNED_BYTE, buffer);
	}
	else if(_bpp == 3){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, pixelDataFormat, GL_UNSIGNED_BYTE, buffer);
	}
	else{
		std::string error = "Texture creation error, byte per pixel error, _bpp: "+_bpp;
		throw std::exception(error.c_str());
	}

	delete[] buffer;
	buffer = NULL;

	return _textureID;
}

GLuint ATexture::Create( GLubyte* buffer, unsigned int contentWidth, unsigned int contentHeight, unsigned int width, unsigned int height, GLenum pixelDataFormat, unsigned char bpp/*=4*/ )
{
	// update texture information
	_width = width;
	_height = height;
	_contentWidth = contentWidth;
	_contentHeight = contentHeight;
	_bpp = bpp;

	//create a new texture id
	glGenTextures(1, &_textureID);
	// Tell OpenGL to use the specific location (&_textureID) to store data
	glBindTexture(GL_TEXTURE_2D, _textureID);

	// Select modulate to mix texture with color for shading
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// TODO: passing in the texture parameters who creation?
	// Since this is a very simple 2D pixelated game, using GL_NEAREST will result a better non-fuzzy image on screen
	// Texture filtering when texture size is big
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Texture filtering when texture size is small
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//create the texture data in opengl
	if(_bpp == 4){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, pixelDataFormat, GL_UNSIGNED_BYTE, buffer);
	}
	else if(_bpp == 3){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, pixelDataFormat, GL_UNSIGNED_BYTE, buffer);
	}
	else{
		std::string error = "Texture loading error, byte per pixel error, _bpp: "+_bpp;
		throw std::exception(error.c_str());
	}

	return _textureID;
}

void ATexture::UpdateBuffer( GLubyte* data )
{
	if(_bpp == 4){
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else if(_bpp == 3){
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else{
		std::string error = "Texture creation error, byte per pixel error, _bpp: "+_bpp;
		throw std::exception(error.c_str());
	}
}

const GLuint ATexture::textureID() const{
	return _textureID;
}

const unsigned int ATexture::contentWidth() const{
	return _contentWidth;
}

const unsigned int ATexture::contentHeight() const{
	return _contentHeight;
}

const unsigned int ATexture::width() const{
	return _width;
}

const unsigned int ATexture::height() const{
	return _height;
}