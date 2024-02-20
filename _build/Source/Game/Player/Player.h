#pragma once
#include "InputHandling.h"
#include "Game/Sprites/Sprite.h"

class Player : public Sprite
{
public:
	void Update();
private:
	Controls mControls;
	Keys mKeys;
};

