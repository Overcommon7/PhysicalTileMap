#include "pch.h"
#include "CameraMovement.h"

#include "Camera/Camera.h"
#include "Game/Player/Player.h"

void CameraMovement::Update(Render::Camera& camera, Values& values, const Player& player)
{
	auto& rCamera = camera.GetRaylibCamera();
	const auto& pRigidbody = player.GetRigidbody();

	rCamera.target = player.GetPosition();
}
