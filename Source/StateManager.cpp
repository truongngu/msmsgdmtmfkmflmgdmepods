#include <algorithm>
#include "StateManager.h"
#include "Utils.h"
#include "DebugDefine.h"
using namespace std;

StateManager* StateManager::instance=0;


StateManager::StateManager()
{
	mListState.clear();
	currentState=0;
}

StateManager::~StateManager()
{
	int n=mListState.size();
	for (int i=0;i<n;i++)
	{
		Delete(mListState[i]);
	}
}

StateManager* StateManager::GetInstance()
{
	if (instance==0)
		instance=new StateManager();
	return instance;
}

void StateManager::AddState(State* state)
{
	mListState.erase(std::remove(mListState.begin(), mListState.end(), state), mListState.end());
	mListState.push_back(state);
}

void StateManager::SwitchState(State* state, bool isReleaseOldState)
{
	AddState(state);
	if (state == 0 || state->IsRelease())
	{
		LogOut("Switch State not succes");
		return;
	}
	State* prev = currentState;
	if (!state->IsInit())
	{
		state->Init();
	}
	currentState = state;
	if (isReleaseOldState)
	{
		if (prev)
		{
			prev->SetRelease(true);
			mListState.erase(std::remove(mListState.begin(), mListState.end(), prev), mListState.end());
			Delete(prev);
			//mListState
		}
	}
}

void StateManager::SwitchToNextState(bool isReleaseCurrentState){
	if (currentState == 0 || currentState->IsRelease())
	{
		Log("Current is released or not exist!");
		return;
	}
	State* prev = currentState;
	currentState = currentState->GetNextState();
	if (isReleaseCurrentState)
	{
		if (prev)
		{
			prev->SetRelease(true);
			mListState.erase(std::remove(mListState.begin(), mListState.end(), prev), mListState.end());
			Delete(prev);
			//mListState
		}
	}
}

State* StateManager::FindState(string Name)
{
	int n=mListState.size();
	if (n==0)
		return 0;
	vector<State*>::iterator tex=find_if(mListState.begin(),mListState.end(),MatchStringState(Name));
	if (tex!=mListState.end())
	{
		auto index = distance(mListState.begin(), tex);
		return mListState.at(index);
	}
	return 0;
}

