#include "ATextureCache.h"
#include "ATexture.h"

ATextureCache* ATextureCache::INSTANCE = NULL;

ATextureCache::ATextureCache(void){

}


ATextureCache::~ATextureCache(void){
	_texturesMap.clear();
}

void ATextureCache::Init(){
	
}

std::tr1::shared_ptr<ATexture> ATextureCache::Cache(const std::string& $key, ATexture* tex)
{
	std::tr1::shared_ptr<ATexture> texture_sp = std::tr1::shared_ptr<ATexture>(tex);
	_texturesMap[$key] = texture_sp;

	return texture_sp;
}

bool ATextureCache::TryRemove(const std::string& $key){
	Map::iterator itr = _texturesMap.find($key);
	if(itr != _texturesMap.end()){
		// "itr->second" is the shared pointer of the ATexture instance.
		// Since the texture map will always contains a reference to the texture shared pointer
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

std::tr1::shared_ptr<ATexture> ATextureCache::Get( const std::string& key )
{
	Map::const_iterator itr = _texturesMap.find(key);
	if(itr != _texturesMap.end()){
		return itr->second;
	}

	return nullptr;
}
