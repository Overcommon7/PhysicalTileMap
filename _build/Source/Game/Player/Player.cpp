#include "pch.h"
#include "Player.h"

#include "Game/Physics/PhysicsWorld.h"
#include "ImGuiUtilities/ImGuiUtils.h"

#include "App/App.h"
#include "Game/Game.h"

Player::Player(Vector2 position, Vector2 size)
	: Sprite(Sprite::Type::Player, position, size, PINK)
	, mRigidbody(this, PhysicsLayer::PLAYER)
	, mControls({})
	, mMovementValues("Debug/MovementValues.txt")
	, mKeys({})
{


	mFixedUpdate = std::move([this](const float timeStep) {
		this->FixedUpdate(timeStep);
		});

	mRigidbody.AddLayerToLayerMask(PhysicsLayer::ENEMIES);
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

void Player::FixedUpdate(float timeStep)
{
	if (mDebug.useInputs)
		Movement::FixedUpdate(this, mMovementValues, timeStep);

	CheckForEnemyCollision();
	CheckForTileCollision();
	
}

void Player::CheckForEnemyCollision()
{
	const auto& collisions = mRigidbody.GetCollisions();
	if (collisions.empty()) return;

	auto it = std::ranges::find_if(collisions, [](const Collision& c) { return c.rigidbody->GetLayer() == PhysicsLayer::ENEMIES; });
	if (it == collisions.end()) return;

	App::GetGameLayer()->Restart();
}

void Player::CheckForTileCollision()
{
	for (int i = 0; i <= (int)CollisionEdge::Left; ++i)
	{
		const auto& tileData = mRigidbody.GetTileData((CollisionEdge)i);
		for (const auto& data : tileData)
		{
			if (!data.mIsDeath)
				continue;

			App::GetGameLayer()->Restart();
			return;
		}
	}
}
