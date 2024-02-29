#include "pch.h"
#include "Movement.h"

#include "Game/Player/Player.h"
#include "ImGuiUtilities/ImGuiUtils.h"

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
	if (values.canStartJump)
		ApplyVerticalForce(player, values, timeStep);
}

void Movement::TurnAround(Player* player, Values& values, const float timeStep)
{
	const Controls& controls = player->mControls;

	if (controls.mThisFrame.leftPressed ||
		controls.mThisFrame.rightReleased && controls.mIsHoldingLeft)
	{
		values.facing = Direction::Left;
		player->mFlipX = false;
	}

	if (controls.mThisFrame.rightPressed ||
		controls.mThisFrame.leftReleased && controls.mIsHoldingRight)
	{
		values.facing = Direction::Right;
		player->mFlipX = true;
	}


}

void Movement::ApplyHorizantalForce(Player* player, Values& values, const float timeStep)
{
	const Controls& controls = player->mControls;
	Rigidbody& rigidbody = player->mRigidbody;
	Vector2 velocity(rigidbody.GetVelocity());

	if (std::abs(velocity.x) < std::numeric_limits<float>::epsilon())
		values.moving = Direction::None;
	else values.moving = velocity.x < 0 ? Direction::Left : Direction::Right;

	if (controls.mIsHoldingRight || controls.mIsHoldingLeft)
	{
		if (values.moving == values.facing)
		{
			float speed = controls.mIsHoldingRun ? values.runningAcclerationSpeed : values.walkingAcclerationSpeed;
			if (values.facing == Direction::Left) speed = -speed;
			rigidbody.ApplyForceX(speed);	
		}
		else
		{
			float speed = controls.mIsHoldingRun ? values.runningDecclerationSpeed : values.walkingDecclerationSpeed;
			if (values.facing == Direction::Left) speed = -speed;
			rigidbody.ApplyForceX(speed);
		}
	}
	else if (controls.mThisFrame.rightReleased || controls.mThisFrame.leftReleased)
	{
		rigidbody.SetDecelerationSpeed(values.decclerationSpeed);
	}

	velocity = rigidbody.GetVelocity();
	rigidbody.SetVelocityX((std::clamp(velocity.x, -values.maxSpeed, values.maxSpeed)));
}

void Movement::ValidateJump(Player* player, Values& values, const float timeStep)
{
	Rigidbody& rigidbody = player->mRigidbody;
	bool isGrounded = rigidbody.IsGrounded();

	if (!isGrounded)
		values.canStartJump = false;

	if (!values.canStartJump && !isGrounded)
	{
		float airTime = Time::Now() - values.timeOfJump;
		if (airTime >= values.maxAirTime)
			values.isJumpActive = false;
	}

	if (!values.canStartJump && isGrounded)
	{
		values.canStartJump = true;
		values.isJumpActive = true;
	}
		

}

void Movement::ApplyVerticalForce(Player* player, Values& values, const float timeStep)
{
	Rigidbody& rigidbody = player->mRigidbody;
	Controls& controls = player->mControls;

	if (controls.mThisFrame.jumpPressed && values.canStartJump)
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

void Movement::ImGuiDraw(Values& values)
{
	ImGuiUtils::SerializeFloat("Max Speed", values.maxSpeed, 0.1f);
	ImGuiUtils::SerializeFloat("Walking Acceleration", values.walkingAcclerationSpeed, 0.1f);
	ImGuiUtils::SerializeFloat("Running Acceleration", values.runningAcclerationSpeed, 0.1f);
	ImGuiUtils::SerializeFloat("Active Walking Decceleration", values.walkingDecclerationSpeed, 0.1f);
	ImGuiUtils::SerializeFloat("Active Running Decceleration", values.runningDecclerationSpeed, 0.1f);
	ImGuiUtils::SerializeFloat("Passive Decceleration", values.decclerationSpeed, 0.1f);
	ImGuiUtils::SerializeFloat("Jump Force", values.initialJumpForce, 0.1f);
	ImGuiUtils::SerializeFloat("Air Time", values.maxAirTime, 0.1f);
	
	ImGuiUtils::DrawBool("Can Jump", values.canStartJump);
	ImGuiUtils::DrawBool("Is Jump Active", values.isJumpActive);
	ImGuiUtils::DrawFloat("Time Of Jump", values.timeOfJump);
	ImGuiUtils::DrawString("Facing", magic_enum::enum_name(values.facing).data());
	ImGuiUtils::DrawString("Moving", magic_enum::enum_name(values.moving).data());
}
