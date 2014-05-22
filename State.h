#pragma once
#include "IStateMachine.h"
#include <list>
#include <iostream>
#include "Renderer.h"
#include "PhysicsContainer.h"
#include <functional>
#include "ResourceManager.h"

class State : public IState
{
	friend class StateMachine;
public:
	vector<State*> GetNeighbors()
	{
		return mNeighbors;
	}


	/// <summary>
	/// Initializes a new instance of the FunnerState class.
	/// </summary>
	/// <param name="mName">Name to display for this state</param>
	/// <param name="mDescription">Text to display for this state</param>
	State(string mName, string mDescription) : IState(mName, mDescription)
	{
		reinitialized = false;
	}

	virtual bool ContainsTransition(State* i)
	{
		return (find(mNeighbors.begin(), mNeighbors.end(), i) != mNeighbors.end());
	}

	virtual string GetName() const
	{
		return mName;
	}

	virtual bool GetTransitionInitSettings(State* transition)
	{
		vector<State*>::iterator it = find(mNeighbors.begin(), mNeighbors.end(), transition);
		if (it != mNeighbors.end())
		{
			return mInits.at(*(mInits.begin() + (it - mNeighbors.begin())));
		}
		return true;
	}

	virtual string TransitionTriggered()
	{
		std::function<bool()> fptr;
		vector<std::function<bool()>>::iterator iter = mConditions.begin();
		while(iter != mConditions.end())
		{
			fptr = *iter;
			if (fptr())
				return (*(mNeighbors.begin() + (iter - mConditions.begin())))->GetName();
			iter++;
		}
		return "";

	}

	virtual void Run()
	{
		// We don't do any fancy stuff, just print out where we are
	}
protected:
	vector<State*> mNeighbors;
	vector<std::function<bool()>> mConditions;
	vector<bool> mInits;
	Renderer* renderer;
	PhysicsContainer* physics;
	Environment* environment;

	Environment* GetEnvironment()
	{
		return environment;
	}

	virtual bool AddTransition(State* i, bool reinit, std::function<bool()> &fptr)
	{
		if (find(mNeighbors.begin(), mNeighbors.end(), i) != mNeighbors.end())
			return false;
		mNeighbors.push_back(i);
		mInits.push_back(reinit);
		mConditions.push_back(fptr);
		return true;
	}

	virtual bool RemoveTransition(State* i)
	{
		vector<State*>::iterator iter = find(mNeighbors.begin(), mNeighbors.end(), i);
		if (iter != mNeighbors.end())
		{
			mConditions.erase(mConditions.begin() + (iter - mNeighbors.begin()));
			mInits.erase(mInits.begin() + (iter - mNeighbors.begin()));
			mNeighbors.erase(iter);
			return true;
		}
		return false;
	}
};