#include "pch.h"
#include "Movement.h"

#include "Game/Player/Player.h"
#include "ImGuiUtilities/ImGuiUtils.h"

//Rigidbody& rigidbody = player->mRigidbody;
//Controls& controls = player->mControls;

void Movement::Update(Player* player, Values& values)
{

}

void Movement::FixedUpdate(Player* player, Values& values, const float timeStep)
{
	UpdateHoriantalMovement(player, values, timeStep);
}

void Movement::UpdateHoriantalMovement(Player* player, Values& values, const float timeStep)
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
	}
	else if (controls.mIsHoldingRight)
	{
		rigidbody.ApplyForceX(acceleration);
	}

	rigidbody.SetVelocityX(std::clamp(rigidbody.GetVelocity().x, -maxSpeed, maxSpeed));
}

void Movement::ImGuiDraw(Values& values)
{
	ImGuiUtils::SerializeFloat("Max Speed", values.maxWalkingSpeed, 0.1f);
	ImGuiUtils::SerializeFloat("Max Speed", values.maxRunningSpeed, 0.1f);
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
	inFile >> decclerationSpeed;
	inFile >> initialJumpForce;
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
	inFile << decclerationSpeed << '\n';
	inFile << initialJumpForce << '\n';
	inFile << maxAirTime << '\n';

	inFile.close();

}
