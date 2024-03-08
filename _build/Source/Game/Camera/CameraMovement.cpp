#include "pch.h"
#include "CameraMovement.h"

#include "Camera/Camera.h"
#include "Game/Player/Player.h"

#include "ImGuiUtilities/ImGuiUtils.h"

#include "Game/Physics/Rigidbody.h"
#include "Game/Sprites/Sprite.h"

void CameraMovement::FixedUpdate(Render::Camera& camera, Values& values, const Player& player)
{
	auto& rCamera = camera.GetRaylibCamera();
	const auto& pRigidbody = player.GetRigidbody();
	auto position = pRigidbody.GetSprite()->GetPosition();

	rCamera.target = position;
	values.positionDelta = Vector2Subtract(position, values.lastPosition);
	values.lastPosition = position;
	rCamera.offset = camera.GetResolution() / 2;
	
}

void CameraMovement::ImGuiDraw(Render::Camera& camera, Values* values)
{
	if (!ImGui::CollapsingHeader("Camera Movement"))
		return;

	const auto& rCamera = camera.GetRaylibCamera();
	ImGuiUtils::DrawVector2("Target: ", rCamera.target);
	ImGuiUtils::DrawVector2("Offset: ", rCamera.offset);
	ImGuiUtils::DrawVector2("Position Delta: ", values->positionDelta);
}
