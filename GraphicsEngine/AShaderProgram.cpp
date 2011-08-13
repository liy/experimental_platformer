#include "AShaderProgram.h"
#include <GL/glew.h>
#include <GL/glu.h>
#include <iostream>

AShaderProgram::AShaderProgram(void)
{
	id = glCreateProgram();
}


AShaderProgram::~AShaderProgram(void)
{
	glDeleteProgram(id);
}

void AShaderProgram::Activate()
{
	// FIXME: Should I link the program here, or separated it out?
	glLinkProgram(id);

	// check linking status
	GLint status;
	glGetProgramiv (id, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(id, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Link failure: %s\n", strInfoLog);
		delete[] strInfoLog;

		activated = false;
		return;
	}


	// start using the program
	glUseProgram(id);

	activated = true;
}

void AShaderProgram::Deactivate()
{
	glUseProgram(0);
	activated = false;
}
