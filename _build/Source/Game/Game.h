#pragma once
#include "App/ILayer.h"
#include "Windows/GameWindow.h"

class Game : public ILayer
{
public:

private:
	unique_ptr<GameWindow> game;
};

