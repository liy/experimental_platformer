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
	//bind the texture
	ATextureManager::GetInstance()->Bind(_texture_sp->fileName());

	glPushMatrix();

	// do the transformation
	glTranslatef(x, y, z);//normal position translation transform
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);//rotation transform
	glScalef(_scale.x, _scale.y, 1.0f);// scale transform
	glTranslatef(-_rect.width*_anchorRatio.x, -_rect.height*_anchorRatio.y, 0.0f);//anchor translation transform

	//enable to use coordinate array as a source texture
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	int vertexSize = sizeof(Vertex3f);
	int startAddr = (int)_vertices;

	// Setup the texture coordinate
	int offset = 0;
	glTexCoordPointer(2, GL_FLOAT, vertexSize, (void*)(startAddr + offset));

	// Setup vertex coordinate
	offset = offsetof(Vertex3f, v);
	glVertexPointer(3, GL_FLOAT, vertexSize, (void*)(startAddr + offset));

	// setup colour
	offset = offsetof(Vertex3f, colour);
	glColorPointer(4, GL_UNSIGNED_BYTE, vertexSize, (void*)(startAddr + offset));

	// draw the two triangles
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, _indices);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}

// Draw the image using a specific matrix transformation
void ASprite::Draw(const Mat4f& mat){
	//bind the texture
	ATextureManager::GetInstance()->Bind(_texture_sp->fileName());

	glPushMatrix();
	// apply the transformation after anchor translation.
	glLoadMatrixf(mat);

	//enable to use coordinate array as a source texture
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	int vertexSize = sizeof(Vertex3f);
	int startAddr = (int)_vertices;

	// Setup the texture coordinate
	int offset = 0;
	glTexCoordPointer(2, GL_FLOAT, vertexSize, (void*)(startAddr + offset));

	// Setup vertex coordinate
	offset = offsetof(Vertex3f, v);
	glVertexPointer(3, GL_FLOAT, vertexSize, (void*)(startAddr + offset));

	// setup colour
	offset = offsetof(Vertex3f, colour);
	glColorPointer(4, GL_UNSIGNED_BYTE, vertexSize, (void*)(startAddr + offset));

	// draw the two triangles
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, _indices);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glPopMatrix();

	int error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "OpenGL error: " << error << "\n";
	}
}

void ASprite::Draw(){
	TransMatrices* matrices = TransMatrices::Instance();

	glBindVertexArray(_vaoID);

	matrices->Push();
	matrices->modelView = matrices->modelView * _transform;

	GLuint modelViewUnifo = glGetUniformLocation(AShaderManager::GetInstance()->activatedProgramID, "modelView");
	glUniformMatrix4fv(modelViewUnifo, 1, GL_FALSE, matrices->modelView);

	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, _indices);

	matrices->Pop();

	glBindVertexArray(0);
}

void ASprite::CreateVBO(void)
{
	int vertexSize = sizeof(Vertex3f);
	// since we are using the VBO, the glBufferData already copied the data into graphic card's memory
	// and the pointer pointed at the started of the memory, so no need to get the _vertice's memory.
	int startAddr = 0;
	int bufferSize = vertexSize * sizeof(_indices)/sizeof(GLubyte);

	glGenBuffers(1, &_vboID);

	// Not sure what the vertex array for
	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);

	// create the vbo
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, _vertices, GL_DYNAMIC_DRAW);

	// vertex position information
	int offset = offsetof(Vertex3f, v);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, vertexSize, (void*)(startAddr+offset));
	// vertex colour information
	offset = offsetof(Vertex3f, colour);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexSize, (void*)(startAddr+offset));
	// This points the vertex position to vertex shader's  location 0 variable. 
	// This points the colour to vertex shader's location 1 variable.
	// TODO: comment needs rephrase
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	GLenum ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: Could not create a VBO: %s \n",
			gluErrorString(ErrorCheckValue)
			);

		exit(-1);
	}
}