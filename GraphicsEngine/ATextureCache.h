#pragma once

#include <iostream>
#include <map>

class ATexture;

class ATextureCache
{
	// In order automate the texture release processs, shared_ptr is used.
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
	 * 
	 */
	std::tr1::shared_ptr<ATexture> Cache( const std::string& $key, ATexture* tex );

	std::tr1::shared_ptr<ATexture> Get(const std::string& key);

	// Remove texture can use the file name's reference.
	bool TryRemove(const std::string& $key);

protected:
	ATextureCache(void);
	~ATextureCache(void);

	Map _texturesMap;
};

