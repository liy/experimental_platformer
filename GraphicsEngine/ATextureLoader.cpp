#include "ATextureLoader.h"
#include <windows.h>
#include <GL\GL.h>
#include <IL\il.h>
#include <IL\ilu.h>
#include "acMath.h"
#include "ATexture.h"
#include "ATextureCache.h"

ATextureLoader::ATextureLoader(void)
{
}


ATextureLoader::~ATextureLoader(void)
{

}

std::tr1::shared_ptr<ATexture> ATextureLoader::LoadFile(const std::string& path)
{
	// try to retrieve the texture from the cache, if it exists
	std::tr1::shared_ptr<ATexture> texture_sp = ATextureCache::GetInstance()->Get(path);
	if(texture_sp != nullptr){
		return texture_sp;
	}

	unsigned int width, contentWidth, height, contentHeight;

	ILuint imageID = ilGenImage();
	ilBindImage(imageID);

	if(!ilLoadImage(path.c_str())){
		std::string error = "Fail to load file: " + path;
		throw std::exception(error.c_str());
		return nullptr;
	}

	// The content in
	width = contentWidth = ilGetInteger(IL_IMAGE_WIDTH);
	height = contentHeight = ilGetInteger(IL_IMAGE_HEIGHT);
	ILint bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);

	// Actual texture  size padded with extra pixels, ensure width and height are power of two.
	if(!isPowerOfTwo(contentWidth))
		width = nextPowerOfTwo(contentWidth);
	if(!isPowerOfTwo(contentHeight))
		height = nextPowerOfTwo(contentHeight);

	// default background colour will be solid black
	ilClearColour(0.0f, 0.0f, 0.0f, 1.0f);

	// TODO: there is still some confusion here.......
	// flip texture problem is mentioned here: http://www.gamedev.net/topic/308200-devil-textures-upside-down/

	// Together with the ilOriginFunc in the graphics_engine.h initialize function, and followed by iLuFlipImage(). 
	// They ensure the image data will be correctly loaded and place on top left corner. And the data will be always stored from top left corner.
	iluImageParameter(ILU_PLACEMENT, ILU_UPPER_LEFT);
	// bitmap image seems like storing data upside down, its origin is on the lower left.
	// jpg, png data seems like using upper left as the origin.
	if (ilGetInteger(IL_IMAGE_ORIGIN) == IL_ORIGIN_UPPER_LEFT){
		// This is for fixing the loaded image upside down in OpenGL...
		iluFlipImage();
	}

	// set the canvas size.
	iluEnlargeCanvas(width, height, bpp);

	// Allocate the memory for the image data.
	GLubyte* buffer = new GLubyte[width * height * bpp];
	// Copy the loaded image data into the texture data depending on how many bytes per pixel
	if(bpp == 4){
		ilCopyPixels(0, 0, 0, width, height, 1, IL_RGBA, GL_UNSIGNED_BYTE, buffer);
	}
	else if(bpp == 3){
		ilCopyPixels(0, 0, 0, width, height, 1, IL_RGB, GL_UNSIGNED_BYTE, buffer);
	}
	else{
		std::string error = "Loading process, byte per pixel error, bpp: "+bpp;
		throw std::exception(error.c_str());	
	}

	// Delete the devIL image data
	ilDeleteImage(imageID);

	// create a brand new texture to use
	// put the texture into the texture cache.
	texture_sp = ATextureCache::GetInstance()->Cache(path, new ATexture(buffer, contentWidth, contentHeight, width, height, GL_RGBA, bpp));

	// after texture is created, the buffer data will be uploaded to OpenGL, so no long needed.
	delete[] buffer;

	// This is a pointer to the loaded image data
	return texture_sp;
}
