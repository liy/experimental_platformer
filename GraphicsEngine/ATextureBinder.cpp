#include "ATextureBinder.h"
#include "ATextureCache.h"
#include "ATexture.h"
#include "gl\glew.h"

ATextureBinder* ATextureBinder::INSTANCE = NULL;

ATextureBinder::ATextureBinder(void)
{
}


ATextureBinder::~ATextureBinder(void)
{
}

void ATextureBinder::Bind( const std::string& key )
{
	std::tr1::shared_ptr<ATexture> texture_sp = ATextureCache::GetInstance()->Get(key);
	if(texture_sp != nullptr){
		GLuint id = texture_sp->GetTextureID();

		// _boundedID == 0 ensures that always bind something when nothing is bounded
		if(_boundedID != id || _boundedID == 0){
			_boundedID = id;

			// OpenGL 3.1 + style of texture binding.
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _boundedID);
		}
	}
}

void ATextureBinder::Bind( const GLuint textureID )
{
	if(_boundedID != textureID){
		_boundedID = textureID;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _boundedID);
	}
}

const GLuint ATextureBinder::BoundedID() const
{
	return _boundedID;
}
