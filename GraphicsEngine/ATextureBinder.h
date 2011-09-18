#pragma once

#include <iostream>

typedef unsigned int GLuint;

class ATextureBinder
{
private:
	static ATextureBinder* INSTANCE;

public:
	static ATextureBinder* GetInstance(){
		if(INSTANCE == NULL){
			INSTANCE = new ATextureBinder();
		}
		return INSTANCE;
	}

	void Bind(const std::string& key);

	void Bind(const GLuint textureID);

	const GLuint BoundedID() const;

protected:
	ATextureBinder(void);
	~ATextureBinder(void);

	GLuint _boundedID;
};

