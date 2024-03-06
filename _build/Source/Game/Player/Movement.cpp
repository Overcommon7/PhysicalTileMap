#include "pch.h"
#include "Movement.h"

#include "Game/Player/Player.h"
#include "ImGuiUtilities/ImGuiUtils.h"

//Rigidbody& rigidbody = player->mRigidbody;
//Controls& controls = player->mControls;

void Movement::Update(Player* player, Values& values)
{
	ValidateJump(player, values);
}

void Movement::FixedUpdate(Player* player, Values& values, const float timeStep)
{
	UpdateHoriantalMovement(player, values, timeStep);
	DoJump(player, values, timeStep);

}

void Movement::UpdateHoriantalMovement(Player* player, Values& values, const float timeStep)
{
	Rigidbody& rigidbody = player->mRigidbody;
	Controls& controls = player->mControls;
	const float velocity = rigidbody.GetVelocity().x;

	if (!controls.mIsHoldingRight && !controls.mIsHoldingLeft)
	{
		values.moving = Direction::None;
	}

	if ((velocity >= 0 && controls.mIsHoldingRight) || (velocity <= 0 && controls.mIsHoldingLeft))
	{
		values.moving = velocity > 0 ? Direction::Right : Direction::Left;
		ApplyAcceleration(player, values, timeStep);
	}
	else
	{
		ApplyDecceleration(player, values, timeStep);
	}

}

void Movement::ApplyAcceleration(Player* player, Values& values, const float timeSte)
{
	Rigidbody& rigidbody = player->mRigidbody;
	Controls& controls = player->mControls;

	float acceleration = values.walkingAcclerationSpeed;
	float maxSpeed = values.maxWalkingSpeed;
	if (controls.mIsHoldingRun)
	{
		acceleration = values.runningAcclerationSpeed;
		maxSpeed = values.maxRunningSpeed;	
	}

	if (controls.mIsHoldingLeft)
	{
		rigidbody.ApplyForceX(-acceleration);
		player->mFlipX = true;
		values.facing = Direction::Left;
	}
	else if (controls.mIsHoldingRight)
	{
		rigidbody.ApplyForceX(acceleration);
		player->mFlipX = false;
		values.facing = Direction::Right;
	}

	rigidbody.SetVelocityX(std::clamp(rigidbody.GetVelocity().x, -maxSpeed, maxSpeed));
}

void Movement::ApplyDecceleration(Player* player, Values& values, const float timeSte)
{
	Rigidbody& rigidbody = player->mRigidbody;
	Controls& controls = player->mControls;

	float deceleration{};

	if (values.moving == Direction::None)
	{
		deceleration = values.passiveDecclerationSpeed;
	}
	else if (controls.mIsHoldingRun)
	{
		deceleration = values.runningDecclerationSpeed;
	}
	else
	{
		deceleration = values.walkingDecclerationSpeed;
	}

	rigidbody.SetDecelerationSpeed(deceleration);
}

void Movement::ValidateJump(Player* player, Values& values)
{
	Controls& controls = player->mControls;
	Rigidbody& rigidbody = player->mRigidbody;

	if (rigidbody.IsGrounded() && !controls.mIsHoldingJump)
	{
		values.canStartJump = true;
	}

	if (values.isJumpActive && rigidbody.IsGrounded())
	{
		values.isJumpActive = false;
	}

	if (values.canStartJump && controls.mThisFrame.jumpPressed)
	{
		values.timeOfJump = Time::Now();
	}
}

void Movement::DoJump(Player* player, Values& values, float fixedTimeStep)
{
	Rigidbody& rigidbody = player->mRigidbody;
	Controls& controls = player->mControls;

	if (!controls.mIsHoldingJump)
		return;
		
	float airTime = Time::Now() - values.timeOfJump;
	if (airTime >= values.maxAirTime) return;

	values.canStartJump = false;

	if (!values.isJumpActive)
	{
		rigidbody.ApplyForceY(-values.initialJumpForce);
		values.isJumpActive = true;
	}
	else
	{
		float t = airTime / values.maxAirTime;
		float jumpForce = std::lerp(values.maintainedJumpForce, 0.f, t);
		rigidbody.ApplyForceY(-jumpForce);
	}


}

void Movement::ImGuiDraw(Values& values)
{
	ImGuiUtils::SerializeFloat("Max Walking Speed", values.maxWalkingSpeed, 0.1f);
	ImGuiUtils::SerializeFloat("Max Running Speed", values.maxRunningSpeed, 0.1f);
	ImGuiUtils::SerializeFloat("Walking Acceleration", values.walkingAcclerationSpeed, 0.1f);
	ImGuiUtils::SerializeFloat("Running Acceleration", values.runningAcclerationSpeed, 0.1f);
	ImGuiUtils::SerializeFloat("Active Walking Decceleration", values.walkingDecclerationSpeed, 0.1f);
	ImGuiUtils::SerializeFloat("Active Running Decceleration", values.runningDecclerationSpeed, 0.1f);
	ImGuiUtils::SerializeFloat("Passive Decceleration", values.passiveDecclerationSpeed, 0.1f);
	ImGuiUtils::SerializeFloat("Initial Jump Force", values.initialJumpForce, 0.1f);
	ImGuiUtils::SerializeFloat("Maitained Jump Force", values.maintainedJumpForce, 0.1f);
	ImGuiUtils::SerializeFloat("Air Time", values.maxAirTime, 0.1f);
	ImGuiUtils::SerializeBool("Apply Maintained Jump Force", values.mDebug.applyForce);
	
	ImGuiUtils::DrawBool("Can Jump", values.canStartJump);
	ImGuiUtils::DrawBool("Is Jump Active", values.isJumpActive);
	ImGuiUtils::DrawFloat("Time Of Jump", values.timeOfJump);
	if (values.isJumpActive)
		ImGuiUtils::DrawFloat("Air Time", Time::Now() - values.timeOfJump);
	else
		ImGui::Text("Air Time 0.00");
	ImGuiUtils::DrawString("Facing", magic_enum::enum_name(values.facing).data());
	ImGuiUtils::DrawString("Moving", magic_enum::enum_name(values.moving).data());

	if (ImGui::Button(ImGuiUtils::GetLabel("Save")))
	{
		values.SaveToFile();
	}
}

Movement::Values::Values(const fs::path& path)
{
	ifstream inFile(path);

	if (!inFile.is_open())
		return;

	inFile >> maxWalkingSpeed;
	inFile >> maxRunningSpeed;
	inFile >> walkingAcclerationSpeed;
	inFile >> runningAcclerationSpeed;
	inFile >> walkingDecclerationSpeed;
	inFile >> runningDecclerationSpeed;
	inFile >> passiveDecclerationSpeed;
	inFile >> initialJumpForce;
	inFile >> maintainedJumpForce;
	inFile >> maxAirTime;	
}

void Movement::Values::SaveToFile()
{
	if (fs::exists(mPath))
		fs::remove(mPath);

	ofstream inFile(mPath);

	inFile << maxWalkingSpeed << '\n';
	inFile << maxRunningSpeed << '\n';
	inFile << walkingAcclerationSpeed << '\n';
	inFile << runningAcclerationSpeed << '\n';
	inFile << walkingDecclerationSpeed << '\n';
	inFile << runningDecclerationSpeed << '\n';
	inFile << passiveDecclerationSpeed << '\n';
	inFile << initialJumpForce << '\n';
	inFile << maintainedJumpForce << '\n';
	inFile << maxAirTime << '\n';

	inFile.close();

}
