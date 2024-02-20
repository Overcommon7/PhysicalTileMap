#pragma once
#include "InputHandling.h"
#include "Movement.h"

#include "Game/Sprites/Sprite.h"
#include "Game/Physics/Rigidbody.h"

class Player : public Sprite
{
public:
	void Update();
private:
	Controls mControls;
	Keys mKeys;
	Rigidbody mRigidbody;
	Movement::Values mMovementValues;

	friend class Movement;
};

