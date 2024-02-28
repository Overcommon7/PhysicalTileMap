#include "pch.h"
#include "Player.h"

#include "Game/Physics/PhysicsWorld.h"
#include "ImGuiUtilities/ImGuiUtils.h"

Player::Player(Vector2 position, Vector2 size)
	: Sprite(Sprite::Type::Player, position, size, PINK)
	, mRigidbody(this, PhysicsLayer::PLAYER)
	, mControls({})
	, mMovementValues({})
	, mKeys({})
{


	/*mFixedUpdate = [this](const float fixedDeltaTime) {
		Movement::Update(this, mMovementValues, fixedDeltaTime);
		};

	PhysicsWorld::OnFixedTimeStep() += mFixedUpdate;*/
}

Player::~Player()
{
	//PhysicsWorld::OnFixedTimeStep() -= mFixedUpdate;
}

void Player::Update()
{
	//InputHandling::Update(mControls, mKeys);	
}

void Player::ImGuiDraw()
{
	if (!ImGui::CollapsingHeader("Player"))
		return;

	Sprite::ImGuiDrawInternal();
	mRigidbody.ImGuiDraw();
}
