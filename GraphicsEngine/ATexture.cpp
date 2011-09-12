#include "ATexture.h"
#include <iostream>
#include <IL\il.h>
#include <IL\ilu.h>
#include "acMath.h"

ATexture::ATexture(const std::string& $fileName): _textureID(0), _contentWidth(0), _contentHeight(0), _data(NULL)
{
	Create($fileName);
}

ATexture::~ATexture(void)
{
	std::cout << "*** ATexture[" << _fileName << ", " << _textureID <<"] destroy ***\n";
	glDeleteTextures(1, &_textureID);
}


GLuint ATexture::Create(const std::string& $fileName){
	// std::string's "assignment operator" is called, basically create a copy of the file name string.
	_fileName = $fileName;

	ILuint imageID = ilGenImage();
	ilBindImage(imageID);

	if(!ilLoadImage(_fileName.c_str())){
		std::string error = "Fail to load file: "+$fileName;
		throw std::exception(error.c_str());
		return NULL;
	}

	// The content in
	_contentWidth = ilGetInteger(IL_IMAGE_WIDTH);
	_contentHeight = ilGetInteger(IL_IMAGE_HEIGHT);
	_bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);

	// Actual texture  size padded with extra pixels, ensure width and height are power of two.
	_width = nextPowerOfTwo(_contentWidth);
	_height = nextPowerOfTwo(_contentHeight);
	ilClearColour(0.0f, 0.0f, 0.0f, 1.0f);

	// TODO: there is still some confusion here.......
	// flip texture problem is mentioned here: http://www.gamedev.net/topic/308200-devil-textures-upside-down/

	// Together with the ilOriginFunc in the ATextureManager init function, and following iLuFlipImage(). 
	// They ensures the image data will be correctly loaded and place on top left corner. And the data will be always stored from top left corner.
	iluImageParameter(ILU_PLACEMENT, ILU_UPPER_LEFT);
	// bitmap image seems like storing data upside down, its origin is on the lower left.
	// jpg, png data seems like using upper left as the origin.
	if (ilGetInteger(IL_IMAGE_ORIGIN) == IL_ORIGIN_UPPER_LEFT){
		// This is for fixing the loaded image upside down in OpenGL...
		iluFlipImage();
	}

	// set the canvas size.
	iluEnlargeCanvas(_width, _height, _bpp);

	// Allocate the memory for the image data.
	_data = new GLubyte[_width*_height*_bpp];
	// Copy the loaded image data into the texture data depending on how many bytes per pixel
	if(_bpp == 4){
		ilCopyPixels(0, 0, 0, _width, _height, 1, IL_RGBA, GL_UNSIGNED_BYTE, _data);
	}
	else if(_bpp == 3){
		ilCopyPixels(0, 0, 0, _width, _height, 1, IL_RGB, GL_UNSIGNED_BYTE, _data);
	}
	else{
		std::string error = "Loading process, byte per pixel error, _bpp: "+_bpp;
		throw std::exception(error.c_str());	
	}

	// Delete the devIL image data
	ilDeleteImage(imageID);

	// Start create OpenGL texture from here

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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
	}
	else if(_bpp == 3){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);
	}
	else{
		std::string error = "Binding process, byte per pixel error, _bpp: "+_bpp;
		throw std::exception(error.c_str());
	}

	// delete data after loading and creation of OpenGL image data
	delete[] _data;
	_data = NULL;

	return _textureID;
}

const GLuint ATexture::textureID() const{
	return _textureID;
}

const std::string& ATexture::fileName() const{
	return _fileName;
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