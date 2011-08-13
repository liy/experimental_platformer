#include "gl\glew.h"
#include "ASprite.h"
#include "ATextureManager.h"
#include "ATexture.h"
#include "acMath.h"

ASprite::ASprite(void): ATextureNode(){
	// initialize the indices array
	_indices = new GLubyte[4];
	_indices[0] = 3;
	_indices[1] = 0;
	_indices[2] = 2;
	_indices[3] = 1;
}

ASprite::ASprite(const std::string& $fileName): ATextureNode($fileName){
	// initialize the indices array
	_indices = new GLubyte[4];
	_indices[0] = 3;
	_indices[1] = 0;
	_indices[2] = 2;
	_indices[3] = 1;
}

ASprite::ASprite(const std::string& $fileName, const Recti& $rect): ATextureNode($fileName, $rect){
	// initialize the indices array
	_indices = new GLubyte[4];
	_indices[0] = 3;
	_indices[1] = 0;
	_indices[2] = 2;
	_indices[3] = 1;
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
	//Draw(_transform);

	//glDrawArrays(GL_TRIANGLES, 0, 3);


	glPushMatrix();
	// apply the transformation after anchor translation.
	glLoadMatrixf(_transform);

	CreateVBO();

	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, _indices);

	glPopMatrix();
}

void ASprite::CreateVBO(void)
{
	/*
	GLfloat Vertices[] = {
		0.0f, 0.0f, 0.0f, 1.0f,
		1.0f,  0.0f, 0.0f, 1.0f,
		0.5f, 1.0f, 0.0f, 1.0f
	};

	GLfloat Colors[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};

	GLenum ErrorCheckValue = glGetError();

	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);

	glGenBuffers(1, &VboId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_DYNAMIC_DRAW);
	// First parameter is the attribute index in the shader file
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &ColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
	// First parameter is the attribute index in the shader file
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	*/

	/*
	_vertices = new Vertex3f[4];
	_vertices[0].colour.Set(255, 0, 0, 255);
	_vertices[0].v.Set(0.0f, 0.0f, 0.0f);

	_vertices[1].colour.Set(0, 255, 0, 255);
	_vertices[1].v.Set(0.5f, 0.0f, 0.0f);

	_vertices[2].colour.Set(0, 0, 255, 255);
	_vertices[2].v.Set(0.5f, 0.5f, 0.0f);

	_vertices[3].colour.Set(0, 0, 0, 255);
	_vertices[3].v.Set(0.0f, 0.5f, 0.0f);
	*/

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

	// vertex shader attribute, vertex information. Setup vertex coordinate
	int offset = offsetof(Vertex3f, v);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, vertexSize, (void*)(startAddr+offset));
	// fragment shader attribute, colour information
	offset = offsetof(Vertex3f, colour);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexSize, (void*)(startAddr+offset));
	// what is this for? enable the two shaders?
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

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