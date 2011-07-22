#pragma once
#include <iostream>
#include <vector>
#include "AImage.h"

class AFrame : public AImage
{
public:
	AFrame(const std::string& $fileName, unsigned int $duration=10);
	AFrame(const std::string& $fileName, const Recti& $rect, unsigned int $duration=10);
	~AFrame(void);

	void Draw(const Vec2f& position, float rotation);

	unsigned int duration;
	unsigned int listIndex;
};

class AAnimation
{
public:
	AAnimation(void);
	AAnimation::AAnimation(const std::vector<AFrame*>& $frames);

	~AAnimation(void);

	bool pingpong;

	bool repeat;

	// direct get and set rotation value is safe.
	float rotation;

	// directly set and get depth is safe
	float depth;

	// TODO: May want to only keep track of width and height for animation instead of scale, since the width and height could be changed during the animation
	float scaleX;
	float scaleY;

	void AddFrame(const std::string& $fileName);

	void AddFrame(const std::string& $fileName, const Recti& $rect);

	void AddFrame(const std::string& $fileName, const Recti& $rect, unsigned int $duration);

	void Update();

	void Draw(const Vec2f& position, float rotation);

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

	void setAnchor(float $xRatio, float $yRatio);
	Vec2f& anchor();

	void setWidth(float $w);
	void setHeight(float $h);
	void setSize(float $w, float $h);

	const float width();
	const float height();
protected:
	static const int ANI_FORWARD = 1;
	static const int ANI_BACKWARD = -1;

	std::vector<AFrame*> _frames;

	unsigned int _frameIndex;

	unsigned int _frameTimer;

	bool _firstRound;

	bool _stopped;

	int _direction;

	Vec2f _anchor;
	// anchor ratio internal use only
	Vec2f _anchorRatio;
};

