#pragma once

#include <iostream>
#include <memory>

class ATexture;

class ATextureLoader
{
public:
	ATextureLoader(void);
	~ATextureLoader(void);

	std::tr1::shared_ptr<ATexture> LoadFile(const std::string& path);
};

