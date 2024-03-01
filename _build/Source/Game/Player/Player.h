#pragma once
#include "InputHandling.h"
#include "Movement.h"

#include "Game/Sprites/Sprite.h"
#include "Game/Physics/Rigidbody.h"

class Player : public Sprite
{
public:
	Player(Vector2 position, Vector2 size);
	~Player();
	void Update();

	void ImGuiDraw();
private:
	struct Debug
	{
		bool useInputs = true;
	};

	Controls mControls;
	Keys mKeys;
	Rigidbody mRigidbody;
	Movement::Values mMovementValues;
	Debug mDebug;

	std::function<void(const float)> mFixedUpdate;

	friend struct Movement;
};

