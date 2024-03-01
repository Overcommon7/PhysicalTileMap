#pragma once
#include "App/ILayer.h"
class GameWindow;
class DebugWindow;
class Project;

class Game final : public ILayer
{
public:
	Game();
	void OnStart(Project* project);
	void Stop();

	void Update() override;
	void RaylibDraw() override;
private:

	void HandleHotkeys();

	GameWindow* mGame;
	DebugWindow* mDebug;
	Project* mProject;
};

