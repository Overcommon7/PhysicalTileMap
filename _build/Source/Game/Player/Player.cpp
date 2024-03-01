#include "pch.h"
#include "Player.h"

#include "Game/Physics/PhysicsWorld.h"
#include "ImGuiUtilities/ImGuiUtils.h"

Player::Player(Vector2 position, Vector2 size)
	: Sprite(Sprite::Type::Player, position, size, PINK)
	, mRigidbody(this, PhysicsLayer::PLAYER)
	, mControls({})
	, mMovementValues("Debug/MovementValues.txt")
	, mKeys({})
{


	mFixedUpdate = [this](const float fixedDeltaTime) {
		if (mDebug.useInputs)
			Movement::FixedUpdate(this, mMovementValues, fixedDeltaTime);
		};

	PhysicsWorld::OnFixedTimeStep() += mFixedUpdate;
}

Player::~Player()
{
	PhysicsWorld::OnFixedTimeStep() -= mFixedUpdate;
}

void Player::Update()
{
	if (!mDebug.useInputs)
		return;

	InputHandling::Update(mControls, mKeys);
	Movement::Update(this, mMovementValues);
}

void Player::ImGuiDraw()
{
	if (!ImGui::CollapsingHeader("Player"))
		return;

	ImGuiUtils::SerializeBool("Use Inputs", mDebug.useInputs);

	Sprite::ImGuiDrawInternal();
	mRigidbody.ImGuiDraw();
	Movement::ImGuiDraw(mMovementValues);
}
