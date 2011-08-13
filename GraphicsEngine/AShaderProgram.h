#pragma once

typedef unsigned int GLuint;

class AShaderProgram
{
public:
	AShaderProgram(void);
	~AShaderProgram(void);

	void Activate();
	void Deactivate();

	bool activated;

	GLuint id;
};

