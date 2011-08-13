#pragma once
#include <iostream>

/// @class Shader
/// @brief and encapsulation of an OpenGL Shader object with
/// associations for source code, etc.
/// Used in conjunction with the ShaderProgram class
/// @author Jonathan Macey
/// @version 1.0
/// @date 24/11/10

enum ShaderType{
	Vertex,
	Fragment,
	Geometry,
	Tessellation,
};

typedef unsigned int GLuint;

class AShader
{
public:

	AShader(ShaderType type);
	~AShader(void);

	/**
	 *	Load a shader file in.
	 */
	void Load(const std::string& fn, const std::string& dir);

	void Compile();

	/**
	 *	The generated shader id when call glCreateShader()
	 */
	GLuint id;

protected:
	/**
	 *	The shader source file name. (without the directory)
	 */
	std::string _fileName;

	/**
	 *	The shader source string
	 */
	std::string* _source;

	/**
	 *	Whether this shader is compiled or not
	 */
	bool _compiled;

	/**
	 *	Shader type
	 */
	ShaderType _type;
};

