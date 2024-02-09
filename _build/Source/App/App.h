#pragma once
#include "ILayer.h"

class App
{
public:
	App();
	~App();
	bool Run();
private:
	enum class State
	{
		Editing = 0,
		Gameplay
	};
private:
	static inline bool isCreated = false;
	vector<unique_ptr<ILayer>> layers;
	bool shouldRestart = false;

	State state;
};

