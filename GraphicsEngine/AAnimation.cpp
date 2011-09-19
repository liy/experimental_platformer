#include "gl\glew.h"
#include "AAnimation.h"
#include "ATexture.h"
#include "acMath.h"
#include "TransMatrices.h"
#include "ATextureBinder.h"

AAnimation::AAnimation(const std::string& $fileName, const std::vector<AFrame*>& $frames):ATextureNode($fileName), _frameIndex(0), pingpong(false), repeat(true), _direction(1), _firstRound(true), _stopped(true),  _frameTimer(1)
{
	_numVertices = 4;

	_indices = new GLubyte[4];
	_indices[0] = 3;
	_indices[1] = 0;
	_indices[2] = 2;
	_indices[3] = 1;

	// TODO: no need to loop through, use assignment constructor??
	int len = $frames.size();
	for(int i=0; i<len; ++i){
		_frames.push_back($frames[i]);
		_frames[i]->index = i;
	}

	SetRect(_frames[_frameIndex]->rect);

	CreateVBO();
}

AAnimation::~AAnimation(void)
{
	_frames.clear();
}

void AAnimation::CreateVBO(void)
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

void AAnimation::AddFrame(const std::string& $fileName, unsigned short $duration, const Vec2f& anchorRatio){
	AFrame* frame = new AFrame($fileName, $duration);
	frame->anchorRatio = anchorRatio;
	frame->index = _frames.size();
	_frames.push_back(frame);

	// this ensures the rect  is set when the first frame is added.
	if(_frameIndex == 0 && _frames.size() == 1){
		SetRect(_frames[_frameIndex]->rect);
	}
}

void AAnimation::AddFrame(const Recti& $rect, unsigned short $duration, const Vec2f& anchorRatio, const std::string& $fileName){
	AFrame* frame = new AFrame($rect, $duration, $fileName);
	frame->anchorRatio = anchorRatio;
	frame->index = _frames.size();
	_frames.push_back(frame);

	// this ensures the rect  is set when the first frame is added.
	if(_frameIndex == 0 && _frames.size() == 1){
		SetRect(_frames[_frameIndex]->rect);
	}
}

void AAnimation::Update(unsigned short delta){
	// jump to next frame
	if(!_stopped){
		if(_frameTimer > _frames[_frameIndex]->duration){
			NextFrame();
			// update the _texture_sp
			if(_frames[_frameIndex]->texture_sp != NULL){
				_texture_sp = _frames[_frameIndex]->texture_sp;
			}
		}
		//  current frame duration has not passed end
		// timer increase, since update is always called first, we do not want the animation missing first frame,
		// therefore, we have to put the frame timer counter increase here
		++_frameTimer;
	}
}

void AAnimation::Draw(float x, float y, float z, float rotation){
	// do the transformation
	Mat4f matrix;
	matrix.Translate(x, y, z);//normal position translation transform
	matrix.RotateZ(rotation);//rotation transform
	matrix.Scale(_scale.x, _scale.y, 1.0f);// scale transform
	matrix.Translate(-_rect.width*_anchorRatio.x, -_rect.height*_anchorRatio.y, 0.0f);//anchor translation transform

	Draw(matrix);
}

void AAnimation::Draw(const Mat4f& mat){
	// if nothing in the frame list, do not draw
	if(_frames.size() == 0)
		return;

	AFrame* frame = _frames[_frameIndex];
	SetRect(frame->rect);

	// save the current model view matrix
	TransMatrices* matrices = TransMatrices::Instance();
	matrices->Push();

	// concatenate the transformation
	matrices->modelView = matrices->modelView * mat;

	// set the vertex shader's model view matrix ready for drawing.
	GLuint modelViewUnifo = glGetUniformLocation(AShaderManager::GetInstance()->activatedProgramID, "modelView");
	glUniformMatrix4fv(modelViewUnifo, 1, GL_FALSE, matrices->modelView);

	// bind the vertex array, restore the animation's draw states
	glBindVertexArray(_vaoID);

	// bind texture
	ATextureBinder::GetInstance()->Bind(_texture_sp->GetTextureID());

	// Seems like if we want to update the vbo, only bind vertex array object will not. We need to explicit bind the VBO, then call glBufferSubData to update the VBO
	int vertexSize = sizeof(Vertex3f);
	int bufferSize = vertexSize * sizeof(_indices)/sizeof(GLubyte);
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, _vertices);

	// draw
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, 0);

	// unbind the vertex state
	glBindVertexArray(0);

	// restore original model view matrix.
	matrices->Pop();

	int error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout << "OpenGL error: " << error << "\n";
	}
}

void AAnimation::Draw(){
	Draw(_transform);
}

bool AAnimation::Running(){
	return !_stopped;
}

void AAnimation::Play(){
	_stopped = false;
}

void AAnimation::GoToPlay(unsigned int $index){
	if($index < _frames.size()){
		_frameIndex = $index;
		_stopped = false;
	}
}

void AAnimation::Stop(){
	_stopped = true;
}

void AAnimation::GoToStop(unsigned int $index){
	if($index < _frames.size()){
		_frameIndex = $index;
		_stopped = true;
	}
}

void AAnimation::Reset(){
	_stopped = true;
	_firstRound = true;
	_frameIndex = 0;
}

int AAnimation::direction(){
	return _direction;
}

void AAnimation::setDirection(int $dir){
	if ($dir>=0) {
		_direction = ANI_FORWARD;
	}
	else {
		_direction = ANI_BACKWARD;
	}
}

unsigned int AAnimation::frameIndex(){
	return _frameIndex;
}

AFrame& AAnimation::CurrentFrame(){
	return *_frames[_frameIndex];
}

void AAnimation::NextFrame(){
	// frame duration counter reset to 1
	_frameTimer = 1;
	//next frame.
	_frameIndex+=_direction;
	
	//frame index exceed the limit
	if(_frameIndex > _frames.size()-1 || _frameIndex < 0){
		//keep repeat and pingpong, never stop
		if(pingpong && repeat){
			_direction *= -1;
			//"direction*2" makes sure the last frame or the first frame do not render twice. See below pingpoing && !repeat case.
			_frameIndex += _direction*2;
		}
		//only pingpong once
		else if(pingpong && !repeat){
			//exceeded the index bound for the first time, still need pingpong once.
			if(_firstRound){
				_direction *= -1;
				_firstRound = false;

				//Reset the current frame to the previous frame immediately.
				//For example, if animation started using ani_forward, at this point, direction will be -1, currentFrameIndex will be [frames count].
				//However since frame index [frames count]-1 is already rendered for its delay time period. we need to jump to [frame count]-2,
				//the new currentFrameIndex = [frame count] + -1*2.
				//If animation started using ani_backward, at this point, direction will be 1, currentFrameIndex will be -1. Since frame 0 is
				//already rendered for it delay time period, we need to directly jump to frame 1 which is: currentFrameIndex = -1 + 1*2. Both cases
				//are correct.
				_frameIndex += _direction*2;
			}
			//exceeded frames index bounds again. stop the animation
			else {
				//next round start the animation, means a new round.
				_firstRound = true;
				_stopped = true;

				//because at this point the currentFramIndex is out of bounds already.
				//It can be -1 or [frames count], this further calculation makes sure the frame is not out of bounds.
				//For example, if animation started using ani_forward, at this point, direction will be -1,
				//currentFrameIndex will be -1, so the new currentFrameIndex = -1 - -1 which will be 0.
				//If animation started using ani_backward, at this point, direction will be 1, currentFrameIndex will be [frames count].
				//So the new currentFrameIndex = [frames count]-1. Both cases are correct.
				_frameIndex -= _direction;
			}
		}
		//go back to start frame
		else if(!pingpong && repeat){
			if (_direction == ANI_FORWARD) {
				_frameIndex = 0;
			}
			else {
				_frameIndex = _frames.size()-1;
			}
		}
		//stop
		else {
			_stopped = true;
			//stop at last valid frame.
			_frameIndex -= _direction;
		}
	}
}

AFrame& AAnimation::GetFrame(unsigned int $index){
	return *_frames[$index];
}