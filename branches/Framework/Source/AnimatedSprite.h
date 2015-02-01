#pragma once
#include "Sprite.h"
#include <map>

class AnimatedSprite : public Sprite{
protected:
	map<std::string, vector<int>> registerFrame;
	std::string frameName;
	int currentFrameIndex;
	bool isMoving;
	bool isAnimating;
	bool isLoop;
protected:
public:
	AnimatedSprite();
	~AnimatedSprite();
	virtual void update(float);
	virtual void onUpdateAnimating(float);
	virtual void setFrameName(std::string frameName);
	virtual void addFrame(std::string, vector<int> dirFrame);
	virtual void setFrameName(char* frameName);
	virtual void addFrame(char*, vector<int> dirFrame);
	virtual void animate(char*, bool loop = true);
	virtual string GetFrameName();
	virtual AnimatedSprite* Create();
	virtual AnimatedSprite* Clone();
};