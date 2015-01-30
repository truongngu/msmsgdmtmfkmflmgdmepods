#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(): Sprite(){
	currentFrameIndex = 0;
	eslapedTime = 0;
	registerFrame.clear();
	frameName = "x";
}

AnimatedSprite::~AnimatedSprite(){

}

void AnimatedSprite::update(float deltaTime){
	if (mState == SpriteState::STOPPED)
	{
		currentFrameIndex=0;
		//return;
	}
	else
	{
		eslapedTime += deltaTime;
		if (eslapedTime > 1.0f/mFramesPerSec)
		{
			eslapedTime=0;
			if (registerFrame.count(frameName)>0)
				onUpdateAnimating(deltaTime);
			else{
				iCurrentFrame++;
				if (iCurrentFrame>lastFrame)
					iCurrentFrame = beginFrame;
			}
		}
	}

	if (registerFrame.count(frameName)>0)
		SetCurrentFrame(registerFrame[frameName][currentFrameIndex]);
	
	//} catch (exception e) {
		//System.out.println("Tank01: " + e.getMessage());
	//}

	int ix = iCurrentFrame % mNumSpriteX;
	int iy = iCurrentFrame / mNumSpriteX;

	// Set texture Coordinates displacement
	dx = float(ix) / float(mNumSpriteX);
	dy = float(iy) / float(mNumSpriteY);
	Entity2D::update(deltaTime);
}

void AnimatedSprite::onUpdateAnimating(float deltaTime){
	if (isAnimating){
		currentFrameIndex++;
		int length = 0;
		try {
			vector<int> animate = registerFrame[frameName];
			length = animate.size();
		} catch (exception e) {
			length = 0;
		}
		if (currentFrameIndex >= length) {
			if (!isLoop){
				isAnimating = false;
			}
			currentFrameIndex = 0;
		}
	}
}

void AnimatedSprite::setFrameName(std::string frameName){
	if (frameName.compare(this->frameName) != 0)
		currentFrameIndex = 0;
	this->frameName = frameName;
}

string AnimatedSprite::GetFrameName(){
	return frameName;
}

void AnimatedSprite::addFrame(std::string frameName, vector<int> dirFrame){
	registerFrame[frameName] = dirFrame;
}

void AnimatedSprite::setFrameName(char* frameName){
	setFrameName(std::string(frameName));
}

void AnimatedSprite::addFrame(char* frameName, vector<int> dirFrame){
	registerFrame[frameName] = dirFrame;
}

void AnimatedSprite::animate(char* frame, bool loop){
	isAnimating = true;
	setFrameName(frame);
	isLoop = loop;
}

void AnimatedSprite::CloneAttribute(AnimatedSprite* sp){
	Sprite::CloneAttribute(sp);
	sp->registerFrame.insert(registerFrame.begin(), registerFrame.end());
	sp->frameName = frameName;
	sp->animate((char*)frameName.c_str());
}

AnimatedSprite* AnimatedSprite::Create(){
	return new AnimatedSprite();
}

AnimatedSprite* AnimatedSprite::Clone(){
	AnimatedSprite* sprite = new AnimatedSprite();
	CloneAttribute(sprite);
	return sprite;

}