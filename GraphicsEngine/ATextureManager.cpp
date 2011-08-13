#include "gl\glew.h"
#include "ATextureManager.h"
#include <memory>
#include <IL\il.h>
#include "ATexture.h"

ATextureManager* ATextureManager::INSTANCE = NULL;

ATextureManager::ATextureManager(void) : _textureID(0){

}


ATextureManager::~ATextureManager(void){
	_texturesMap.clear();
}

void ATextureManager::Init(){
	// Initialize DevIL
	ilInit();

	// All the image loaded will be using top left corner as origin.
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
	ilEnable(IL_ORIGIN_SET);

	// initialize glew, so we can use OpenGL 3.3 functions
	GLenum glewInitResult = glewInit();
	if(glewInitResult != GLEW_OK){
		std::cout << glewGetErrorString(glewInitResult) <<"\n"; 
	}
}

std::tr1::shared_ptr<ATexture> ATextureManager::Get(const std::string& $fileName){
	// Find the texture stored in the texture map
	Map::const_iterator itr = _texturesMap.find($fileName);
	if(itr != _texturesMap.end()){
		return itr->second;
	}

	// Not found in the map. Then creat a new texture to load the image file.
	std::tr1::shared_ptr<ATexture> texture_sp;
	try{
		std::cout << "Create Texture\n";
		// Create texture, shared_ptr's assignment operator is called
		texture_sp = std::tr1::shared_ptr<ATexture>(new ATexture($fileName));
	}
	catch(std::exception& $e){
		std::cout << "Texture creation error, cannot find the file: \n" << $fileName;
		throw $e;
	}

	// Add to the map,
	_texturesMap[$fileName] = texture_sp;

	return texture_sp;
}

void ATextureManager::Bind(const std::string& $fileName){
	std::tr1::shared_ptr<ATexture> texture_sp;

	try{
		texture_sp = Get($fileName);
	}
	catch(std::exception& $e){
		std::cout << "texture binding error\n";
		throw $e;
	}

	// Have to always enable the texture 2d just in case it is not enabled.
	glEnable(GL_TEXTURE_2D);

	// If the bounded texture is not the requested texture, bind it; or if the bounded id is 0 which means no texture is bounded then bind the texture
	if(_textureID != texture_sp->textureID() || _textureID == 0){
		_textureID = texture_sp->textureID();
		// Active the texture, in order to draw it onto the screen.
		glBindTexture(GL_TEXTURE_2D, _textureID);
	}
}

void ATextureManager::Bind(const GLuint& textureID){
	if(_textureID != textureID){
		_textureID = textureID;
		glBindTexture(GL_TEXTURE_2D, _textureID);
	}
}

bool ATextureManager::Remove(const std::string& $fileName){
	Map::iterator itr = _texturesMap.find($fileName);
	if(itr != _texturesMap.end()){
		// "itr->second" is the shared pointer of the ATexture instance.
		// Since the texture map will always contains a reference  to the texture shared pointer
		// Therefore if the reference count is less than or equal to 1 (0, reference count should not happen!). We can safely assume that only TextureManager or no one is holding a reference to the 
		// texture anymore, so remove it from the map.
		if(itr->second.use_count() <= 1){
			//std::cout << "remove texture from TextureManager: " << itr->second->fileName() << "!!!!\n";
			_texturesMap.erase(itr);
			return true;
		}
		else{
			std::cout << "refcount: " << itr->second.use_count() <<"\n";
			return false;
		}
	}
	return false;
}

// Return the const reference of the bounded id. Should not be changed
const GLuint& ATextureManager::boundedID() const{
	return _textureID;
}