#pragma once
#include <vector>
#include "Entity2D.h"
#include "StateConfig.h"
using namespace std;

class State
{
private:
protected:
	vector<Entity2D*> mListEntity;
	vector<Entity2D*> mListEntityInView;
	map<std::string, Entity2D*> entityList;
	vector<string> mListSoundName;
	string mName;
	string path2Config;
	string path2SoundList;
	bool isRelease;
	bool isInit;

	State* nextState;
	StateConfig* stateConfig;
public:
	State();
	virtual ~State();
	void SetRelease(bool val){ isRelease = val; }
	bool IsRelease(){ return isRelease; }
	bool IsInit(){ return isInit; }
	string GetName(){ return mName; }
	virtual void Init();
	virtual StateConfig* GetStateConfig();
	virtual void SetStateConfig(StateConfig* newConfig);
	virtual void LoadSound();
	virtual void Mouse(MouseData mouse, bool bIsDown) = 0;
	virtual void Key(unsigned char key, bool bIsPressed) = 0;
	virtual void onTouch(int id, float x, float y) = 0;
	virtual void Update(float deltatime);
	virtual void Draw();
	virtual void PauseAllSound();
	virtual void ResumeSound() = 0;

	State* GetNextState();
	void SetNextState(State*);
	Entity2D* GetEntityByName(std::string);
	Entity2D* GetEntityByIndex(int);
};