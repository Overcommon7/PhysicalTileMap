#pragma once
#include "ILayer.h"
class Game;
class Editor;

class App
{
public:
	enum class State
	{
		Editing = 0,
		Gameplay
	};
public:
	App();
	~App();
	void Run();

	static void ChangeState(State state);
	static void Close();
private:
	static inline bool isCreated = false;
	static inline App* instance = nullptr;
	vector<unique_ptr<ILayer>> layers;

	Game* game;
	Editor* editor;

	std::optional<State> queriedStateChange;
	State state;
	bool shouldClose;
};

