#include "AShaderManager.h"
#include "AShader.h"
#include "AShaderProgram.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

AShaderManager* AShaderManager::INSTANCE = NULL;

AShaderManager::AShaderManager(void)
{
}


AShaderManager::~AShaderManager(void)
{
}

void AShaderManager::CreateShader(const std::string& fn, const std::string& filePath, ShaderType type)
{
	AShader* shader = new AShader(type);
	shader->Load(filePath, "");
	shader->Compile();

	_shaderMap[fn] = shader;
}

void AShaderManager::CreateShaderProgram(const std::string& name)
{
	_programMap[name] = new AShaderProgram();
}

void AShaderManager::AttachShader(const std::string& shaderName, const std::string& progName)
{
	glAttachShader(_programMap[progName]->id, _shaderMap[shaderName]->id);
}

void AShaderManager::ActivateProgram(const std::string& progName)
{
	AShaderProgram* prog = _programMap[progName];
	prog->Activate();
	activatedProgramID = prog->id;
}

void AShaderManager::DeactivateProgram( const std::string& progName )
{
	_programMap[progName]->Deactivate();
	activatedProgramID = 0;
}

GLuint AShaderManager::GetProgramID(const std::string& progName)
{
	return _programMap[progName]->id;
}
