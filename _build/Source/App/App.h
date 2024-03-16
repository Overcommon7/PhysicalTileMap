#pragma once
#include "ILayer.h"
class Game;
class Editor;

template <class T>
concept CLayer = std::is_base_of_v<ILayer, T>;

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

	static Game* GetGameLayer() { return sInstance->mGame; }
	static Editor* GetEditorLayer() { return sInstance->mEditor; }
private:
	static inline bool sIsCreated = false;
	static inline App* sInstance = nullptr;
	vector<unique_ptr<ILayer>> mLayers;

	Game* mGame;
	Editor* mEditor;

	std::optional<State> mQueriedStateChange;
	State mState;
	bool mShouldClose;
};

