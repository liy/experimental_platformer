#include "ASprite.h"
#include "ATextureManager.h"
#include "ATexture.h"
#include "acMath.h"

ASprite::ASprite(void): ATextureNode(){

}

ASprite::ASprite(const std::string& $fileName): ATextureNode($fileName){

}

ASprite::ASprite(const std::string& $fileName, const Recti& $rect): ATextureNode($fileName, $rect){

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
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, _indices);

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
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, _indices);

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
	Draw(_transform);
}