#pragma once
#include <iostream>
#include <string>
#include <map>

class AShaderProgram;
class AShader;
enum ShaderType;
typedef unsigned int GLuint;

class AShaderManager
{

private:
	static AShaderManager* INSTANCE;

	std::map<std::string, AShader*> _shaderMap;
	std::map<std::string, AShaderProgram*> _programMap;

public:
	static AShaderManager* GetInstance(){
		if(INSTANCE == NULL){
			INSTANCE = new AShaderManager();
		}
		return INSTANCE;
	}

	GLuint activatedProgramID;

	void Init();

	void CreateShader(const std::string& fn, const std::string& filePath, ShaderType type);
	void CreateShaderProgram(const std::string& name);

	/**
	 *	Attach the shader to a program
	 */
	void AttachShader(const std::string& shaderName, const std::string& progName);

	/**
	 *	Start using the program
	 */
	void ActivateProgram(const std::string& progName);
	
	/**
	 *	Stop using the program
	 */
	void DeactivateProgram(const std::string& progName);

	/**
	 *	Get the program id
	 */
	GLuint GetProgramID(const std::string& progName);

protected:
	AShaderManager(void);
	~AShaderManager(void);
};

