#include "StateMachine.h"
#include "GameApplication.h"
#include "Logger.h"
#include "EmbeddedResources.h"

State* StateMachine::CurrentState()
{
	return mCurrent;
}

bool StateMachine::AddState(State* state)
{
	if (!ContainsState(state->GetName()))
	{
		mStates[state->GetName()] = state;
		return true;
	}
	return false;
}

bool StateMachine::RemoveState(State* state)
{
	map<string, State*>::iterator iter = mStates.find(state->GetName());
	if (iter != mStates.end())
	{
		mStates.erase(iter);
		return true;
	}
	return false;
}

bool StateMachine::RemoveState(string state)
{
	map<string, State*>::iterator iter = mStates.find(state);
	if (iter != mStates.end())
	{
		mStates.erase(iter);
		return true;
	}
	return false;
}

bool StateMachine::AddTransition(string from, string to, std::function<bool()> fptr)
{
	if (!(ContainsState(from) && ContainsState(to)))
	{
		return false;
	}
	else
	{
		return mStates[from]->AddTransition(mStates[to], fptr);
	}
}

bool StateMachine::RemoveTransition(string rem, string from)
{
	if (!(ContainsState(rem) && ContainsState(from)))
	{
		return true;
	}
	else
	{
		return mStates[from]->RemoveTransition(mStates[rem]);
	}
}

bool StateMachine::ContainsState(string state)
{
	return (mStates.find(state) != mStates.end());
}

void StateMachine::SetFinal(State* state)
{
	if (ContainsState(state->GetName()))
	{
		mExit = state;
	}
}

void StateMachine::SetFinal(string state)
{
	if (ContainsState(state))
	{
		mExit = mStates[state];
	}
}

vector<string> StateMachine::PossibleTransitions()
{
	vector<string> result;
	for each(State* state in mCurrent->GetNeighbors())
	{
		result.push_back(state->GetName());
	}
	return result;
}

bool StateMachine::Advance(string nextState)
{
	if (ContainsState(nextState))
		/*for each(State* state in mCurrent->GetNeighbors())
		{
		if (nextState == state->GetName())
		{
		state->SetRunning();
		mCurrent = state;
		return true;
		}
		}*/
	{
		mStates[nextState]->SetRunning();
		mCurrent = mStates[nextState];
		return true;
	}
	Logger::Instance().Write(LOG_APP, (unsigned long)ID_INVALID_STATE, nextState);
	return false;
}

bool StateMachine::Start(string name)
{
	if (ContainsState(name))
	{
		mCurrent = mStates[name];
		mCurrent->SetRunning();
		return true;
	}
	return false;
}

bool StateMachine::IsComplete()
{
	return mCurrent == mExit;
}