#pragma once
#include "App/ILayer.h"
class GameWindow;
class Project;

class Game : public ILayer
{
public:
	Game();
	void OnStart(Project* project);
	void Stop();
private:
	GameWindow* game;
};

