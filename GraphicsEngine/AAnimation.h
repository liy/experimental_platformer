#pragma once
#include <iostream>
#include <vector>
#include "AIGraphics.h"

class ATexture;

class AFrame
{
public:
	AFrame(const std::string& $fileName, unsigned short $duration=10);
	AFrame(const std::string& $fileName, const Recti& $rect, unsigned short $duration=10);
	~AFrame(void);

	// The texture which frame is using.
	std::tr1::shared_ptr<ATexture> texture_sp;

	// Specifying 4 corners in the texture coordinate, in counter clock wise ordering.
	// First vertex is the bottom left, second bottom right, third top right, final one is top left.
	// Note its value is 0.0f ~ 1.0f, texture coordinate.
	Vec2f texCoord[4];

	// How many mini seconds this frame is shown
	unsigned short duration;

	// The frame's index position in the animation list.
	unsigned int index;

	// Setup texture coordinate
	void setRect(const Recti& $rect);
	// Setup texture coordinate
	void setRect(int $x, int $y, int $width, int $height);
	// Get the texture coordinate
	const Recti& rect() const;

protected:
	// The actual offset position, width and height, related to the texture resolution.
	Recti _rect;
};

class AAnimation: public AIGraphics
{
public:
	AAnimation(void);
	AAnimation::AAnimation(const std::vector<AFrame*>& $frames);

	~AAnimation(void);

	bool pingpong;

	bool repeat;

	void Update(unsigned short delta);

	// Draw the image to a specific position and rotation
	virtual void Draw(const Vec3f& position, float rotation){
		Draw(position.x, position.y, position.z, rotation);
	}

	// Draw the image to a specific position and rotation
	virtual void Draw(const Vec2f& position, float z, float rotation){
		Draw(position.x, position.y, z, rotation);
	}
	// Draw the image to a specific position and rotation
	void Draw(float x, float y, float z, float rotation);

	void AddFrame(const std::string& $fileName);

	void AddFrame(const std::string& $fileName, const Recti& $rect);

	void AddFrame(const std::string& $fileName, const Recti& $rect, unsigned short $duration);

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

	Vec2f anchor() const;

	void setWidth(float $w);
	void setHeight(float $h);
	void setSize(float $w, float $h);

	const float width() const;
	const float height() const;
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

