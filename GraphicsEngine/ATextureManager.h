#pragma once

#include <iostream>
#include <string>
#include <map>
#include <GL\glut.h>
#include <GL\GL.h>

class ATexture;

class ATextureManager
{
	// In order automate the texture release processs, shared_ptr is used.
	// If the std::tr1::shared_ptr<ATexture> has no reference anymore, it will auto release itself, and also release the associated ATexture instance as well
	typedef std::map<std::string, std::tr1::shared_ptr<ATexture>> Map;

private:
	static ATextureManager* INSTANCE;

public:
	static ATextureManager* GetInstance(){
		if(INSTANCE == NULL){
			INSTANCE = new ATextureManager();
		}
		return INSTANCE;
	}

	// Before create any ATexture instance, you MUST  remember to initialize the manager which will initialize DevIL image loader
	void Init();

	std::tr1::shared_ptr<ATexture> Get(const std::string& $fleName);
	void Bind(const std::string& $fileName);

	/**
	 *	Directly bind the specific texture. Since we already have the id, I assume it is already loaded.
	 */
	void Bind(const GLuint& textureID);

	// Remove texture can use the file name's reference.
	bool Remove(const std::string& $fileName);

	// Return the reference of the bounded id. A const ATextureManager can access this getter method.
	const GLuint& boundedID() const;

protected:
	ATextureManager(void);
	~ATextureManager(void);

	GLuint _textureID;

	Map _texturesMap;
};

