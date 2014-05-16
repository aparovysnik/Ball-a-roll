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
	}

	virtual bool ContainsTransition(State* i)
	{
		return (find(mNeighbors.begin(), mNeighbors.end(), i) != mNeighbors.end());
	}

	virtual string GetName() const
	{
		return mName;
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
		std::cout << "Old McDonalds had a farm eeeyaheeeyahooooh!" << endl;
	}
protected:
	vector<State*> mNeighbors;
	vector<std::function<bool()>> mConditions;
	Renderer* renderer;
	PhysicsContainer* physics;
	Environment* environment;

	Environment* GetEnvironment()
	{
		return environment;
	}

	virtual bool AddTransition(State* i, std::function<bool()> &fptr)
	{
		if (find(mNeighbors.begin(), mNeighbors.end(), i) != mNeighbors.end())
			return false;
		mNeighbors.push_back(i);
		mConditions.push_back(fptr);
		return true;
	}

	virtual bool RemoveTransition(State* i)
	{
		vector<State*>::iterator iter = find(mNeighbors.begin(), mNeighbors.end(), i);
		if (iter != mNeighbors.end())
		{
			mConditions.erase(mConditions.begin() + (iter - mNeighbors.begin()));
			mNeighbors.erase(iter);
			return true;
		}
		return false;
	}
};