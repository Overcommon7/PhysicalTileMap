#include "pch.h"
#include "CameraMovement.h"

#include "Camera/Camera.h"
#include "Game/Player/Player.h"

#include "ImGuiUtilities/ImGuiUtils.h"

#include "Game/Physics/Rigidbody.h"
#include "Game/Sprites/Sprite.h"


void CameraMovement::FixedUpdate(Render::Camera& camera, Values& values, const Player& player, const float timeStep)
{
	camera.GetRaylibCamera().offset = camera.GetResolution() / 2;


	switch (values.mMovementType)
	{
	case CameraMovement::Type::Fixed: 
		Fixed(camera, values, player);
		break;
	case CameraMovement::Type::Smooth:
		Smooth(camera, values, player, timeStep);
		break;
	case CameraMovement::Type::UpdateOnLand:
		UpdateOnLand(camera, values, player, timeStep);
		break;
	case CameraMovement::Type::PlayerPush:
		PlayerPush(camera, values, player);
		break;
	}
}

void CameraMovement::ImGuiDraw(Render::Camera& camera, Values* values)
{
	if (!ImGui::CollapsingHeader("Camera Movement"))
		return;

	static const char* items[] = { magic_enum::enum_name(Type::Fixed).data(), magic_enum::enum_name(Type::PlayerPush).data(), magic_enum::enum_name(Type::Smooth).data(), magic_enum::enum_name(Type::UpdateOnLand).data() };
	static int item_current = (int)values->mMovementType;
	
	if (ImGui::Combo("Movement Type", &item_current, items, IM_ARRAYSIZE(items)))
	{	
		values->mMovementType = magic_enum::enum_cast<Type>(items[item_current]).value_or(Type::Fixed);
	}


	if (values->mMovementType == Type::UpdateOnLand)
	{
		static const char* types[] = { magic_enum::enum_name(HorizantalMoveType::Bounds).data(), magic_enum::enum_name(HorizantalMoveType::Fixed).data(), magic_enum::enum_name(HorizantalMoveType::Smooth).data() };
		static int current = (int)values->mOnLand.moveType;

		if (ImGui::Combo("Horizantal Move Type", &current, types, IM_ARRAYSIZE(types)))
		{
			values->mOnLand.moveType = magic_enum::enum_cast<HorizantalMoveType>(types[current]).value_or(HorizantalMoveType::Fixed);
		}

		ImGuiUtils::SerializeFloat("Evening Out Speed", values->mOnLand.evenOutSpeed);
		ImGuiUtils::DrawFloat("Evening Out Target", values->mOnLand.evenOutTarget);
		ImGuiUtils::DrawBool("Is Evening Out", values->mOnLand.isEveningOut);
	}

	if (values->mMovementType == Type::PlayerPush || 
		(values->mMovementType == Type::UpdateOnLand && values->mOnLand.moveType == HorizantalMoveType::Bounds))
	{
		ImGui::DragFloat("X Threshold", &values->mPush.xMoveThreshold, 0.005f, 0.01f, 0.49f);
		if (values->mMovementType == Type::PlayerPush)
			ImGui::DragFloat("Y Threshold", &values->mPush.yMoveThreshold, 0.005f, 0.01f, 0.49f);

		ImGuiUtils::DrawVector2("Relative Position", values->mRelativePosition);
		ImGuiUtils::DrawVector2("Min", values->mPush.min);
		ImGuiUtils::DrawVector2("Max", values->mPush.max);
	}

	if (values->mMovementType == Type::Smooth ||
		(values->mMovementType == Type::UpdateOnLand && values->mOnLand.moveType == HorizantalMoveType::Smooth))
	{
		ImGuiUtils::SerializeFloat("Distance Effector", values->mSmooth.distanceEffector);
		ImGuiUtils::SerializeFloat("Min Effect Distance", values->mSmooth.minEffectDistance);
		ImGuiUtils::SerializeFloat("Min Speed", values->mSmooth.minSpeed);
	}



	const auto& rCamera = camera.GetRaylibCamera();
	ImGuiUtils::DrawVector2("Target: ", rCamera.target);
	ImGuiUtils::DrawVector2("Offset: ", rCamera.offset);
	ImGuiUtils::DrawVector2("Position Delta: ", values->mPositionDelta);
	ImGuiUtils::DrawVector2("Last Position: ", values->mLastPosition);
}

void CameraMovement::Fixed(Render::Camera& camera, Values& values, const Player& player)
{
	auto& rCamera = camera.GetRaylibCamera();
	values.mLastPosition = rCamera.target;
	rCamera.target = player.GetPosition();
	values.mPositionDelta = Vector2Subtract(rCamera.target, values.mLastPosition);
}

void CameraMovement::Smooth(Render::Camera& camera, Values& values, const Player& player, float timeStep)
{
	auto& rCamera = camera.GetRaylibCamera();
	const Vector2 pPosition = player.GetPosition();
	Vector2 offset = Vector2Subtract(pPosition, rCamera.target);
	float length = Vector2Length(offset);

	if (length > values.mSmooth.minEffectDistance)
	{
		float speed = fmaxf(values.mSmooth.distanceEffector * length, values.mSmooth.minSpeed);
		rCamera.target = Vector2Add(rCamera.target, Vector2Scale(offset, speed * timeStep / length));
	}
}

void CameraMovement::UpdateOnLand(Render::Camera& camera, Values& values, const Player& player, const float timeStep)
{
	const Rigidbody& rigidbody = player.GetRigidbody();
	const Vector2 pPosition = player.GetPosition();
	auto& rCamera = camera.GetRaylibCamera();

	if (values.mOnLand.isEveningOut)
	{
		if (values.mOnLand.evenOutTarget > rCamera.target.y)
		{
			rCamera.target.y += values.mOnLand.evenOutSpeed * timeStep;

			if (rCamera.target.y > values.mOnLand.evenOutTarget)
			{
				rCamera.target.y = values.mOnLand.evenOutTarget;
				values.mOnLand.isEveningOut = false;
			}
		}
		else
		{
			rCamera.target.y -= values.mOnLand.evenOutSpeed * timeStep;

			if (rCamera.target.y < values.mOnLand.evenOutTarget)
			{
				rCamera.target.y = values.mOnLand.evenOutTarget;
				values.mOnLand.isEveningOut = false;
			}
		}
	}
	else if (rigidbody.IsGrounded() && pPosition.y != rCamera.target.y)
	{
		values.mOnLand.isEveningOut = true;
		values.mOnLand.evenOutTarget = pPosition.y;
	}

	if (values.mOnLand.moveType == HorizantalMoveType::Fixed)
	{
		rCamera.target.x = pPosition.x;
	}

	if (values.mOnLand.moveType == HorizantalMoveType::Bounds)
	{
		PlayerPush(camera, values, player, true, false);
	}

	if (values.mOnLand.moveType == HorizantalMoveType::Smooth)
	{
		float yPosition = rCamera.target.y;
		Smooth(camera, values, player, timeStep);
		rCamera.target.y = yPosition;
	}
}

void CameraMovement::PlayerPush(Render::Camera& camera, Values& values, const Player& player, bool doX, bool doY)
{
	auto& rCamera = camera.GetRaylibCamera();
	const Vector2 pPosition = player.GetPosition();
	const Vector2 resolution = camera.GetResolution();
	values.mRelativePosition = GetWorldToScreen2D(pPosition, rCamera);

	values.mPush.max = Vector2Multiply(resolution, { values.mPush.xMoveThreshold, values.mPush.yMoveThreshold });
	values.mPush.min = Vector2Subtract(resolution, values.mPush.max);

	values.mPositionDelta = Vector2Subtract(pPosition, values.mLastPosition);
	values.mLastPosition = pPosition;


	auto CheckPosition = [&](int idx) {
		if ((&values.mRelativePosition.x)[idx] <= (&values.mPush.min.x)[idx] || (&values.mRelativePosition.x)[idx] >= (&values.mPush.max.x)[idx])
		{
			(&rCamera.target.x)[idx] += (&values.mPositionDelta.x)[idx];
		}
	};

	if (doX) CheckPosition(0);
	if (doY) CheckPosition(1);
}
