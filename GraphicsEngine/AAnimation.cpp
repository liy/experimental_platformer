#include "AAnimation.h"
#include "ATextureManager.h"
#include "ATexture.h"
#include "acMath.h"

AFrame::AFrame(const std::string& $fileName, unsigned short $duration): duration($duration)
{
	texture_sp = ATextureManager::GetInstance()->Get($fileName);

	// Set the rect to be the size of the image
	setRect(0, 0, texture_sp->contentWidth(), texture_sp->contentHeight());
}

AFrame::AFrame(const std::string& $fileName, const Recti& $rect, unsigned short $duration): duration($duration)
{
	texture_sp = ATextureManager::GetInstance()->Get($fileName);

	setRect($rect);
}

AFrame::~AFrame(void){
	free(texCoord);

	// keep a copy record of the filename, to remove the 
	std::string fileName = texture_sp->fileName();
	std::cout << "AFrame["<< fileName <<"] destroy!\n";
	// Null the reference, so we can try to remove th texture
	texture_sp = NULL;
	// Try to remove the using texture from the memory.
	// If the reference count is 1.(1 reference count is maintained by the map). Then we remove it from the memory.
	// So programmer will not need to manually  remove texture from the memory
	ATextureManager::GetInstance()->Remove(fileName);
}

void AFrame::setRect(const Recti& $rect){
	// copy assignment
	_rect = $rect;

	// calculate bottom left of the image in texture coordinate. 
	float x = (float)_rect.x/texture_sp->width();
	float y = (float)_rect.y/texture_sp->height();
	// Calculate the the width and height in texture coordinate.
	float w = (float)_rect.width/texture_sp->width();
	float h = (float)_rect.height/texture_sp->height();

	// assign the texture coordinate
	texCoord[0].Set(x, y);
	texCoord[1].Set(x + w, y);
	texCoord[2].Set(x + w, y + h);
	texCoord[3].Set(x, y + h);
}

void AFrame::setRect(int $x, int $y, int $width, int $height){
	_rect.x = $x;
	_rect.y = $y;
	_rect.width = $width;
	_rect.height = $height;

	// calculate bottom left of the image in texture coordinate. 
	float x = (float)_rect.x/texture_sp->width();
	float y = (float)_rect.y/texture_sp->height();
	// Calculate the the width and height in texture coordinate.
	float w = (float)_rect.width/texture_sp->width();
	float h = (float)_rect.height/texture_sp->height();

	// assign the texture coordinate
	texCoord[0].Set(x, y);
	texCoord[1].Set(x + w, y);
	texCoord[2].Set(x + w, y + h);
	texCoord[3].Set(x, y + h);
}

const Recti& AFrame::rect() const{
	return _rect;
}



AAnimation::AAnimation(void): AIGraphics(), _frameIndex(0), pingpong(false), repeat(true), _direction(1), _firstRound(true), _stopped(true), _frameTimer(1)
{
	scale.Set(1.0f, 1.0f);
	anchorRatio.Set(0.5f, 0.5f);
}

AAnimation::AAnimation(const std::vector<AFrame*>& $frames): AIGraphics(), _frameIndex(0), pingpong(false), repeat(true), _direction(1), _firstRound(true), _stopped(true),  _frameTimer(1)
{
	int len = $frames.size();
	for(int i=0; i<len; ++i){
		_frames.push_back($frames[i]);
		_frames[i]->index = i;
	}

	scale.Set(1.0f, 1.0f);
	anchorRatio.Set(0.5f, 0.5f);
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

	ATextureManager::GetInstance()->Bind(frame->texture_sp->fileName());

	glPushMatrix();

	glTranslatef(x, y, 0.0f);//normal position translation transform
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);//rotation transform
	glTranslatef(-width()*anchorRatio.x, -height()*anchorRatio.y, 0.0f);//anchor translation transform
	glScalef(scale.x, scale.y, 1.0f);// scale transform

	// tint the colour
	glColor4f(colour.r, colour.g, colour.b, colour.a);
	
	glBegin(GL_QUADS);
	glTexCoord2f(frame->texCoord[0].x, frame->texCoord[0].y);				glVertex3f(0.0f, 0.0f, z);
	glTexCoord2f(frame->texCoord[1].x, frame->texCoord[1].y);				glVertex3f(frame->rect().width, 0.0f, z);
	glTexCoord2f(frame->texCoord[2].x, frame->texCoord[2].y);				glVertex3f(frame->rect().width, frame->rect().height, z);
	glTexCoord2f(frame->texCoord[3].x, frame->texCoord[3].y);				glVertex3f(0.0f, frame->rect().height, z);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
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

Vec2f AAnimation::anchor() const{
	return Vec2f(width()*anchorRatio.x, height()*anchorRatio.y);
}

void AAnimation::setWidth(float $w){
	scale.x = $w/(float)_frames[_frameIndex]->rect().width;
}

void AAnimation::setHeight(float $h){
	scale.y = $h/(float)_frames[_frameIndex]->rect().height;
}

void AAnimation::setSize(float $w, float $h){
	setWidth($w);
	setHeight($h);
}

const float AAnimation::width() const{
	return (float)(_frames[_frameIndex]->rect().width)*scale.x;
}

const float AAnimation::height() const{
	return (float)(_frames[_frameIndex]->rect().height)*scale.y;
}