#pragma once
#include <string>
#include <vector>

using namespace std;

class IState
{
public:

	IState(string n, string d)
	{
		this->mName = n;
		this->mDescription = d;
	}

	virtual ~IState() {};

	// Utility function to help us display useful things
	virtual string GetName() const = 0;

	//Pre configure on every run
	virtual void SetRunning(bool reinit)
	{
		if (reinit & !reinitialized)
			Init();
		reinitialized = false;
	}

	// Do something
	virtual void Run() = 0;

	virtual void Stop() = 0;


	//Overloading output operator for convenient printing
	inline friend std::ostream& operator<<(std::ostream& o, const IState& v) {
		o << "State: " << v.GetName();
		return o;
	}
protected:
	string mName;
	string mDescription;

	bool reinitialized;

	virtual void Init() { reinitialized = true; }
};

class IStateMachine
{
	// Accessor to look at the current state.
public:
	IStateMachine() {};

	virtual ~IStateMachine() {};

	virtual IState* CurrentState() = 0;

	// List of all possible transitions we can make from this current state.
	virtual vector<string> PossibleTransitions() = 0;

	// Advance to a named state, returning true on success.
	virtual bool Advance(string nextState) = 0;

	// Is this state a "completion" state. Are we there yet?
	virtual bool IsComplete() = 0;
};