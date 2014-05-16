#pragma once
class IGameLogic
{
public:
	IGameLogic() {};
	~IGameLogic() {};

	//Initialization function
	virtual void Init() = 0;

	//Update function to be called on every iteration
	virtual void Update() = 0;
};

