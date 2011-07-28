#include "AAnimation.h"
#include "ATextureManager.h"
#include "ATexture.h"
#include "acMath.h"

AFrame::AFrame(const std::string& $fileName, unsigned int $duration): AImage($fileName),
	duration($duration)
{

}
AFrame::AFrame(const std::string& $fileName, const Recti& $rect, unsigned int $duration): AImage($fileName, $rect),
	duration($duration)
{

}

AFrame::~AFrame(void){

}

void AFrame::Draw(const Vec2f& position, float rotation){
	//std::cout << "draw: " << _texture_sp->fileName() << "\n";
	glPushMatrix();

	//bind the texture
	ATextureManager::GetInstance()->Bind(_texture_sp->fileName());

	// do anchor translation
	_anchor.x = -width()*_anchorRatio.x;
	_anchor.y = -height()*_anchorRatio.y;

	/*
	glTranslatef(position.x, position.y, 0.0f);//normal position translation transform
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);//rotation transform
	glScalef(scaleX, scaleY, 1.0f);// scale transform
	glTranslatef(_anchor.x, _anchor.y, 0.0f);//anchor translation transform
	*/

	glColor4f(1.0f, 1.0f, 1.0f, alpha);
	
	glBegin(GL_QUADS);
	glTexCoord2f(_texOffsetX, _texOffsetY);								glVertex3f(0.0f, 0.0f, depth);
	glTexCoord2f(_texWidth + _texOffsetX, _texOffsetY);					glVertex3f(_rect.width, 0.0f, depth);
	glTexCoord2f(_texWidth + _texOffsetX, _texHeight + _texOffsetY);	glVertex3f(_rect.width, _rect.height, depth);
	glTexCoord2f(_texOffsetX, _texHeight + _texOffsetY);				glVertex3f(0.0f, _rect.height, depth);
	glEnd();
	

	glPopMatrix();
}


AAnimation::AAnimation(void): _frameIndex(0), pingpong(false), repeat(true), _direction(1), _firstRound(true), _stopped(true), rotation(0), scaleX(1.0), scaleY(1.0), _frameTimer(1)
{
	_anchor.x = 0.0f;
	_anchor.y = 0.0f;
	_anchorRatio.x = 0.5f;
	_anchorRatio.y = 0.5f;
}

AAnimation::AAnimation(const std::vector<AFrame*>& $frames): _frameIndex(0), pingpong(false), repeat(true), _firstRound(true), 
	_stopped(true), _direction(1), rotation(0), scaleX(1.0), scaleY(1.0), _frameTimer(1)
{
	int len = $frames.size();
	for(int i=0; i<len; ++i){
		_frames.push_back($frames[i]);
		_frames[i]->listIndex = i;
	}

	_anchor.x = 0.0f;
	_anchor.y = 0.0f;
	_anchorRatio.x = 0.5f;
	_anchorRatio.y = 0.5f;
}

AAnimation::~AAnimation(void)
{
	_frames.clear();
}

void AAnimation::AddFrame(const std::string& $fileName){
	AFrame* frame = new AFrame($fileName);
	frame->listIndex = _frames.size();
	_frames.push_back(frame);
}

void AAnimation::AddFrame(const std::string& $fileName, const Recti& $rect){
	AFrame* frame = new AFrame($fileName, $rect);
	frame->listIndex = _frames.size();
	_frames.push_back(frame);
}


void AAnimation::AddFrame(const std::string& $fileName, const Recti& $rect, unsigned int $duration){
	AFrame* frame = new AFrame($fileName, $rect, $duration);
	frame->listIndex = _frames.size();
	_frames.push_back(frame);
}

void AAnimation::Update(){
	// jump to next frame
	if(!_stopped){
		if(_frameTimer > _frames[_frameIndex]->duration){
			NextFrame();
		}//  current frame duration has not passed end
		// timer increase, since update is always called first, we do not want the animation missing first frame,
		// therefore, we have to put the frame timer counter increase here
		++_frameTimer;
	}
}

void AAnimation::Draw(const Vec2f& position, float rotation){
	// if nothing in the frame list, do not draw
	if(_frames.size() == 0)
		return;

	glPushMatrix();

	// do anchor translation
	_anchor.x = -width()*_anchorRatio.x;
	_anchor.y = -height()*_anchorRatio.y;

	
	glTranslatef(position.x, position.y, 0.0f);//normal position translation transform
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);//rotation transform
	glTranslatef(_anchor.x, _anchor.y, 0.0f);//anchor translation transform
	glScalef(scaleX, scaleY, 1.0f);// scale transform
	//std::cout << "anchor shift: " << _anchor.x << "\n";
	

	_frames[_frameIndex]->Draw(position, rotation);

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
		//only pingpong onec
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
				//So the new currentFrameIndex = [frames count]-1. Both casese are correct.
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

void AAnimation::setAnchor(float $xRatio, float $yRatio){
	// record the ratio, so if we dynamically change texture, we can update the anchor position
	// according to the old ratio
	_anchorRatio.x = $xRatio;
	_anchorRatio.y = $yRatio;
	// record the anchor translation, so we do not need to calculate them in draw function.
	// _anchor.x =- width()*_anchorRatio.x;
	// _anchor.y = -height()*_anchorRatio.y;
}

Vec2f& AAnimation::anchor(){
	return _anchor;
}

void AAnimation::setWidth(float $w){
	scaleX = $w/(float)_frames[_frameIndex]->rect().width;
}

void AAnimation::setHeight(float $h){
	scaleY = $h/(float)_frames[_frameIndex]->rect().height;
}

void AAnimation::setSize(float $w, float $h){
	setWidth($w);
	setHeight($h);
}

const float AAnimation::width(){
	return (float)(_frames[_frameIndex]->width())*scaleX;
}

const float AAnimation::height(){
	return (float)(_frames[_frameIndex]->height())*scaleY;
}