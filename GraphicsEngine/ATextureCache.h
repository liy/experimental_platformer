#pragma once

#include <iostream>
#include <map>

class ATexture;

class ATextureCache
{
	// In order automate the texture release process, shared_ptr is used.
	// If the std::tr1::shared_ptr<ATexture> has no reference anymore, it will auto release itself, and also release the associated ATexture instance as well
	typedef std::map<std::string, std::tr1::shared_ptr<ATexture>> Map;

private:
	static ATextureCache* INSTANCE;

public:
	static ATextureCache* GetInstance(){
		if(INSTANCE == NULL){
			INSTANCE = new ATextureCache();
		}
		return INSTANCE;
	}

	// Before create any ATexture instance, you MUST  remember to initialize the manager which will initialize DevIL image loader
	void Init();

	/**
	 * Cache the texture and key pair. Return the shared pointer of the texture.
	 */
	std::tr1::shared_ptr<ATexture> Cache( const std::string& $key, ATexture* tex );

	/**
	 * Get the cached shared pointer of ATexture. If no such key and value pair is found, nullptr is returned.
	 */
	std::tr1::shared_ptr<ATexture> Get(const std::string& key);

	// Remove texture can use the file name's reference.
	bool TryRemove(const std::string& $key);

protected:
	ATextureCache(void);
	~ATextureCache(void);

	Map _texturesMap;
};

