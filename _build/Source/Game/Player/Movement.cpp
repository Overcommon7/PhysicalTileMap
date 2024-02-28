#include "pch.h"
#include "Movement.h"

#include "Game/Player/Player.h"

//Rigidbody& rigidbody = player->mRigidbody;
//Controls& controls = player->mControls;

void Movement::Update(Player* player, Values& values, const float timeStep)
{
	Move(player, values, timeStep);
	Jump(player, values, timeStep);
}

void Movement::Move(Player* player, Values& values, const float timeStep)
{
	TurnAround(player, values, timeStep);
	ApplyHorizantalForce(player, values, timeStep);
}

void Movement::Jump(Player* player, Values& values, const float timeStep)
{
	ValidateJump(player, values, timeStep);
	if (values.canJump)
		ApplyVerticalForce(player, values, timeStep);
}

void Movement::TurnAround(Player* player, Values& values, const float timeStep)
{
}

void Movement::ApplyHorizantalForce(Player* player, Values& values, const float timeStep)
{
}

void Movement::ValidateJump(Player* player, Values& values, const float timeStep)
{

}

void Movement::ApplyVerticalForce(Player* player, Values& values, const float timeStep)
{
	Rigidbody& rigidbody = player->mRigidbody;
	Controls& controls = player->mControls;

	if (controls.mThisFrame.jumpPressed)
	{
		rigidbody.SetVelocityY(values.initialJumpForce);
		values.timeOfJump = Time::Now();
		values.isJumpActive = true;
		return;
	}

	if (values.isJumpActive && controls.mIsHoldingJump)
	{
		float airTime = Time::Now() - values.timeOfJump;
		float jumpForce = 1 - (airTime / values.maxAirTime);
		rigidbody.ApplyForceY(values.initialJumpForce * jumpForce);
	}
}
