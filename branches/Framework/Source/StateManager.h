#include <vector>
#include "State.h"
using namespace std;


class StateManager
{
private:
	vector<State*> mListState;
	StateManager();
	static StateManager* instance;
	State* currentState;
	void AddState(State*);
protected:
public:
	~StateManager();
	static StateManager* GetInstance();
	State* GetCurrentState(){return currentState;};
	void SwitchState(State*,bool);
	void SwitchToNextState(bool);
	State* FindState(string Name);
};