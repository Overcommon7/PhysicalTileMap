#include "pch.h"
#include "Enemy.h"

#include "Game/Physics/PhysicsWorld.h"
#include "ImGuiUtilities/ImGuiUtils.h"

Enemy::Enemy(Type type, Vector2 position, Vector2 size, Color color)
	: Sprite(type, position, size, color)
	, mRigidbody(this, PhysicsLayer::ENEMIES)
	, mSpeed(60), mFacingLeft(true), mNegativeSpeed(-60)
{
	mRigidbody.UseDeceleration(false);
	mFixedUpdate = std::move([this](const float timeStep) {this->FixedUpdate(timeStep); });
	mRigidbody.SetVelocityX(mNegativeSpeed);

	PhysicsWorld::OnFixedTimeStep() += mFixedUpdate;
}

Enemy::~Enemy()
{
	PhysicsWorld::OnFixedTimeStep() -= mFixedUpdate;
}

void Enemy::ImGuiDraw()
{
	if (ImGui::CollapsingHeader(ImGuiUtils::GetLabel(mName)))
	{
		Sprite::ImGuiDrawInternal();
		if (ImGuiUtils::SerializeFloat("Speed", mSpeed) ||
			ImGuiUtils::SerializeBool("Facing Left", mFacingLeft))
		{
			mRigidbody.SetVelocityX(mFacingLeft ? mNegativeSpeed : mSpeed);
		}	
	}
}

void Enemy::FixedUpdate(const float timeStep)
{
	if (std::abs(mRigidbody.GetVelocity().x) > 0.001f)
		return;

	mFacingLeft = !mFacingLeft;
	mRigidbody.SetVelocityX(mFacingLeft ? mNegativeSpeed : mSpeed);
}
