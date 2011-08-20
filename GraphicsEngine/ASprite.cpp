#include "gl\glew.h"
#include "ASprite.h"
#include "ATextureManager.h"
#include "ATexture.h"
#include "acMath.h"
#include "TransMatrices.h"

ASprite::ASprite(void): ATextureNode(){
	// initialize the indices array
	_indices = new GLubyte[4];
	_indices[0] = 3;
	_indices[1] = 0;
	_indices[2] = 2;
	_indices[3] = 1;

	CreateVBO();
}

ASprite::ASprite(const std::string& $fileName): ATextureNode($fileName){
	// initialize the indices array
	_indices = new GLubyte[4];
	_indices[0] = 3;
	_indices[1] = 0;
	_indices[2] = 2;
	_indices[3] = 1;

	CreateVBO();
}

ASprite::ASprite(const std::string& $fileName, const Recti& $rect): ATextureNode($fileName, $rect){
	// initialize the indices array
	_indices = new GLubyte[4];
	_indices[0] = 3;
	_indices[1] = 0;
	_indices[2] = 2;
	_indices[3] = 1;

	CreateVBO();
}

ASprite::~ASprite(void)
{
	
}

// Draw the image to a specific position and rotation
void ASprite::Draw(float x, float y, float z, float rotation){
	// do the transformation
	Mat4f matrix;
	matrix.Translate(x, y, z);//normal position translation transform
	matrix.RotateZ(rotation);//rotation transform
	matrix.Scale(_scale.x, _scale.y, 1.0f);// scale transform
	matrix.Translate(-_rect.width*_anchorRatio.x, -_rect.height*_anchorRatio.y, 0.0f);//anchor translation transform

	Draw(matrix);
}

// Draw the image using a specific matrix transformation
void ASprite::Draw(const Mat4f& mat){
	// save the current model view matrix
	TransMatrices* matrices = TransMatrices::Instance();
	matrices->Push();

	// concatenate the transformation
	matrices->modelView = matrices->modelView * mat;

	// set the vertex shader's model view matrix ready for drawing.
	GLuint modelViewUnifo = glGetUniformLocation(AShaderManager::GetInstance()->activatedProgramID, "modelView");
	glUniformMatrix4fv(modelViewUnifo, 1, GL_FALSE, matrices->modelView);

	// bind the vertex states
	glBindVertexArray(_vaoID);
	// bind texture
	ATextureManager::GetInstance()->Bind(_texture_sp->fileName());
	// draw
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, 0);
	// unbind the vertex state
	glBindVertexArray(0);

	// restore original model view matrix.
	matrices->Pop();

	GLenum ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"OpenGL error: %s \n",
			gluErrorString(ErrorCheckValue)
			);
	}
}

void ASprite::Draw(){
	Draw(_transform);
}

void ASprite::CreateVBO(void)
{
	int vertexSize = sizeof(Vertex3f);
	int bufferSize = vertexSize * sizeof(_indices)/sizeof(GLubyte);
	
	// initialization for VAO
	// The vertex array basically is ued for saving all the states. So later when drawing, 
	//  instead of bind texture, setup texture coordinate, colour information, etc. we can simply use glBindVertexArray(_vaoID) to set the state and ready for drawing.
	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);



	// create the vbo
	glGenBuffers(1, &_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, _vertices, GL_STATIC_DRAW);

	// This points the vertex position to vertex shader's  location 0 variable. 
	// This points the colour to vertex shader's location 1 variable.
	// Points the texture coordinate to location 2 variable
	// TODO: comment needs rephrase
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// since we are using the VBO, the glBufferData already copied the data into graphic card's memory
	// and the pointer pointed at the started of the memory, so no need to get the _vertice's memory.
	int startAddr = 0;
	// texture
	int offset = 0;	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)startAddr);
	// vertex position information
	offset = offsetof(Vertex3f, v);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, vertexSize, (void*)(startAddr+offset));
	// vertex colour information
	offset = offsetof(Vertex3f, colour);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexSize, (void*)(startAddr+offset));

	// create ibo
	glGenBuffers(1, &_iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);



	// unbind the vao
	glBindVertexArray(0);

	GLenum ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: Could not create a VBO: %s \n",
			gluErrorString(ErrorCheckValue)
			);

		//exit(-1);
	}
}