#include "AAnimation.h"
#include "ATextureManager.h"
#include "ATexture.h"
#include "acMath.h"

AAnimation::AAnimation(void): ATextureNode(), _frameIndex(0), pingpong(false), repeat(true), _direction(1), _firstRound(true), _stopped(true), _frameTimer(1)
{
	_numVertices = 4;
	_vertices = new Vertex3f[_numVertices];

	_scale.Set(1.0f, 1.0f);
	_anchorRatio.Set(0.5f, 0.5f);
}

AAnimation::AAnimation(const std::vector<AFrame*>& $frames): ATextureNode(), _frameIndex(0), pingpong(false), repeat(true), _direction(1), _firstRound(true), _stopped(true),  _frameTimer(1)
{
	_numVertices = 4;
	_vertices = new Vertex3f[_numVertices];

	// TODO: no need to loop through, use assignment constructor??
	int len = $frames.size();
	for(int i=0; i<len; ++i){
		_frames.push_back($frames[i]);
		_frames[i]->index = i;
	}

	_scale.Set(1.0f, 1.0f);
	_anchorRatio.Set(0.5f, 0.5f);
}

AAnimation::~AAnimation(void)
{
	_frames.clear();
}

void AAnimation::AddFrame(const std::string& $fileName){
	AFrame* frame = new AFrame($fileName);
	frame->index = _frames.size();
	_frames.push_back(frame);
}

void AAnimation::AddFrame(const std::string& $fileName, const Recti& $rect){
	AFrame* frame = new AFrame($fileName, $rect);
	frame->index = _frames.size();
	_frames.push_back(frame);
}


void AAnimation::AddFrame(const std::string& $fileName, const Recti& $rect, unsigned short $duration){
	AFrame* frame = new AFrame($fileName, $rect, $duration);
	frame->index = _frames.size();
	_frames.push_back(frame);
}

void AAnimation::Update(unsigned short delta){
	// jump to next frame
	if(!_stopped){
		if(_frameTimer > _frames[_frameIndex]->duration){
			NextFrame();
		}
		//  current frame duration has not passed end
		// timer increase, since update is always called first, we do not want the animation missing first frame,
		// therefore, we have to put the frame timer counter increase here
		++_frameTimer;
	}
}

void AAnimation::Draw(float x, float y, float z, float rotation){
	// if nothing in the frame list, do not draw
	if(_frames.size() == 0)
		return;

	AFrame* frame = _frames[_frameIndex];
	_rect = frame->rect;

	ATextureManager::GetInstance()->Bind(frame->texture_sp->fileName());

	glPushMatrix();

	// since there's no transformation before this line, we can safely  directly load the input matrix
	glLoadMatrixf(_transform);
	// The anchor translation transform will be concatenated
	// inputMatrix * anchorTransMatrix * vertices
	glTranslatef(-GetWidth()*_anchorRatio.x, -GetHeight()*_anchorRatio.y, 0.0f);

	// TODO: draw

	//enable to use coordinate array as a source texture
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	int vertexSize = sizeof(Vertex3f);
	int startAddr = (int)&_vertices[0];

	// Setup the vertex pointer first
	// offset of the vertex vector position.
	// Since the Vertex3f extends Vec3f, the first offset for vertex pointer array will be 0
	int offset = 0;
	glVertexPointer(3, GL_FLAT, vertexSize, (void*)(startAddr + offset));

	// Setup the texture coordinate
	offset = offsetof(Vertex3f, uv);
	glTexCoordPointer(2, GL_FLOAT, vertexSize, (void*)(startAddr + offset));

	// setup colour
	offset = offsetof(Vertex3f, colour);
	glColorPointer(4, GL_UNSIGNED_BYTE, vertexSize, (void*)(startAddr + offset));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, _indices);

	glPopMatrix();
}

void AAnimation::Draw(){

}

void AAnimation::Draw(const Mat4f& mat){

}

/*
void AAnimation::Draw(const Mat4f& mat){
	// if nothing in the frame list, do not draw
	if(_frames.size() == 0)
		return;

	AFrame* frame = _frames[_frameIndex];

	ATextureManager::GetInstance()->Bind(frame->texture_sp->fileName());

	glPushMatrix();

	// since there's no transformation before this line, we can safely  directly load the input matrix
	glLoadMatrixf(mat);
	// The anchor translation transform will be concatenated
	// inputMatrix * anchorTransMatrix * vertices
	glTranslatef(-width()*_anchorRatio.x, -height()*_anchorRatio.y, 0.0f);

	// TODO: draw

	glPopMatrix();
}
*/

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

