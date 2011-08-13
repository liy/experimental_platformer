#include "AShader.h"
#include <GL/glew.h>
#include <GL/glu.h>
#include <iostream>
#include <fstream>

AShader::AShader(ShaderType type)
{
	_type = type;
	switch (type)
	{
		case Fragment:
			id = glCreateShader(GL_FRAGMENT_SHADER_ARB);
			break;
		case Geometry:
			id = glCreateShader(GL_GEOMETRY_SHADER_ARB);
			break;
		case Tessellation:
			id = glCreateShader(GL_TESS_CONTROL_SHADER);
			break;
		case Vertex:
			id = glCreateShader(GL_VERTEX_SHADER_ARB);
			break;
	}
}


AShader::~AShader(void)
{
	glDeleteShader(id);
}

void AShader::Load( const std::string& fn, const std::string& dir )
{
	std::ifstream shaderSource(fn.c_str());
	if (!shaderSource.is_open())
	{
		std::cerr << "File not found " << fn.c_str() << "\n";
		exit(EXIT_FAILURE);
	}

	// read in the data
	_source = new std::string((std::istreambuf_iterator<char>(shaderSource)), std::istreambuf_iterator<char>());
	shaderSource.close();
	*_source += "\0";

	const char* data = _source->c_str();
	glShaderSource(id , 1, &data, NULL);
	_compiled = false;
}

void AShader::Compile()
{
	glCompileShader(id);

	// check shader compile result
	GLint status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE){
		GLint inforLogLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &inforLogLength);

		const char* shaderTypeStr = NULL;
		switch(_type){
			case Vertex: 
				shaderTypeStr = "Vertex"; 
				break;
			case Geometry: 
				shaderTypeStr = "Geometry"; 
				break;
			case Fragment:
				shaderTypeStr = "Fragment";
				break;
			case Tessellation:
				shaderTypeStr = "Tessellation";
				break;
		}

		// +1 is for final ending character
		GLchar* strInfoLog = new GLchar[inforLogLength + 1];
		glGetShaderInfoLog(id, inforLogLength, NULL, strInfoLog);

		fprintf(stderr, "Compile failure in %s shader 'name of the shader':\n %s\n", shaderTypeStr, strInfoLog);
		delete[] strInfoLog;

		_compiled = false;
	}

	_compiled = true;
}