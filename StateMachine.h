#pragma once
#include "IStateMachine.h"
#include <list>
#include "Singleton.h"
#include "State.h"
#include "GameOnState.h"
#include "GamePausedState.h"
#include "GameOverState.h"
#include "GameWonState.h"
#include "GameSetupState.h"
#include <functional>
#include <string>

using namespace std;
class StateMachine : public IStateMachine, public Singleton<StateMachine>
{

	friend class Singleton<StateMachine>;

public:
	
	State* CurrentState();

	virtual bool AddState(State* state);

	virtual bool RemoveState(State* state);

	virtual bool RemoveState(string state);

	virtual bool AddTransition(string from, string to, bool reinit, std::function<bool()> fptr);

	virtual bool RemoveTransition(string rem, string from);

	virtual bool ContainsState(string state);

	virtual void SetFinal(State* state);

	virtual void SetFinal(string state);

	bool Start(string state);

	vector<string> PossibleTransitions();
	
	bool Advance(string nextState);
	
	bool IsComplete();
protected:
	StateMachine() { mExit = NULL; }
	virtual ~StateMachine() {
		if (mCurrent != NULL)
			delete mCurrent;
		if (mExit != NULL)
			delete mExit;
		mStates.clear();
	};
	map<string, State*> mStates;
	State* mCurrent;
	State* mExit;
};

