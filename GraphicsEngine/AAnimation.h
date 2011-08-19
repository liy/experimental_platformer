#pragma once
#include <iostream>
#include <vector>
#include "ATextureManager.h"
#include "ATextureNode.h"

struct AFrame{
	/**
	 *	The texture which frame is using.
	 */
	std::tr1::shared_ptr<ATexture> texture_sp;

	/**
	 *	How many mini seconds this frame is shown
	 */
	unsigned short duration;

	/**
	 *	The frame's index position in the animation list.
	 */
	unsigned int index;

	/**
	 *	Each frame has its own anchor shift, so we can place them freely.
	 */
	Vec2f anchorRatio;

	/**
	 *	The  rectangle for generate texture coordinate.
	 */
	Recti rect;

	AFrame(const std::string& $fileName, unsigned short $duration=10): duration($duration){
		texture_sp = ATextureManager::GetInstance()->Get($fileName);
		rect.Set(0, 0, texture_sp->contentWidth(), texture_sp->contentHeight());
		anchorRatio.Set(0.5f, 0.5f);
	}

	AFrame(const Recti& $rect, unsigned short $duration=10, const std::string& $fileName=""): duration($duration), rect($rect){
		if($fileName != ""){
			texture_sp = ATextureManager::GetInstance()->Get($fileName);
		}
		else{
			texture_sp = NULL;
		}
		anchorRatio.Set(0.5f, 0.5f);
	}

	Vec2f GetAnchor() const{
		return Vec2f(rect.width*anchorRatio.x, rect.height*anchorRatio.y);
	}

	~AFrame(void){
		// TODO: how to free the texture coordinate

		// keep a copy record of the filename, to remove the 
		if(texture_sp != NULL){
			std::string fileName = texture_sp->fileName();
			std::cout << "AFrame["<< fileName <<"] destroy!\n";
			// Null the reference, so we can try to remove th texture
			texture_sp = NULL;
			// Try to remove the using texture from the memory.
			// If the reference count is 1.(1 reference count is maintained by the map). Then we remove it from the memory.
			// So programmer will not need to manually  remove texture from the memory
			ATextureManager::GetInstance()->Remove(fileName);
		}
	}
};

class AAnimation: public ATextureNode
{
public:

	/**
	 *	
	 */
	AAnimation::AAnimation(const std::string& $fileName, const std::vector<AFrame*>& $frames);

	~AAnimation(void);

protected:

	//void Init(const Recti& $rect);

public:

	bool pingpong;

	bool repeat;

	/**
	 *	If you do not provide clip rectangle, you must provide a texture, otherwise the rectangle cannot be correctly set. The clip rectangle will be the size of the texture
	 */
	void AddFrame(const std::string& $fileName, unsigned short $duration=10, const Vec2f& anchroRatio=Vec2f(0.5f, 0.5f));

	/**
	 *	Frame's texture is optional, if you do not provide one. The default texture will be using AAnimation's texture. If you do provide one, it will override the AAnimation's texture.
	 */
	void AddFrame(const Recti& $rect, unsigned short $duration=10, const Vec2f& anchroRatio=Vec2f(0.5f, 0.5f), const std::string& $fileName="");

	void Update(unsigned short delta);

	// Draw the image to a specific position and rotation
	void Draw(float x, float y, float z, float rotation);

	// Draw the image using a specific matrix transformation
	void Draw(const Mat4f& mat);

	// Draw the image to a specific position and rotation
	void Draw();

	void Play();

	void GoToPlay(unsigned int $index);

	void Stop();

	void GoToStop(unsigned int $index);

	void Reset();

	bool Running();

	int direction();

	void setDirection(int $dir);

	unsigned int frameIndex();

	AFrame& CurrentFrame();

	void NextFrame();

	AFrame& GetFrame(unsigned int $index);

	void CreateVBO(void);

private:
	GLuint _vaoID, _vboID;

protected:
	static const int ANI_FORWARD = 1;
	static const int ANI_BACKWARD = -1;

	std::vector<AFrame*> _frames;

	unsigned int _frameIndex;

	unsigned int _frameTimer;

	bool _firstRound;

	bool _stopped;

	int _direction;
};

