#include "pch.h"
#include "CameraMovement.h"

#include "Camera/Camera.h"
#include "Game/Player/Player.h"

#include "ImGuiUtilities/ImGuiUtils.h"

#include "Game/Physics/Rigidbody.h"
#include "Game/Sprites/Sprite.h"

void CameraMovement::Update(Render::Camera& camera, Values& values, const Player& player)
{
	auto& rCamera = camera.GetRaylibCamera();
	const auto& pRigidbody = player.GetRigidbody();

	rCamera.target = values.sprite->GetPosition();
	rCamera.offset = camera.GetResolution() * 0.5f;

	values.rigidbody->SetVelocity(pRigidbody.GetVelocity());

}

void CameraMovement::ImGuiDraw(Values* values)
{
	if (!ImGui::CollapsingHeader("Camera Movement"))
		return;

	
}

void CameraMovement::Values::Initialize()
{
	sprite = new Sprite(Sprite::Type::Other, Vector2{ 0, 0 }, Vector2{ 1, 1 });
	rigidbody = new Rigidbody(sprite, PhysicsLayer::CAMERA, false, true);
	rigidbody->UseDeceleration(false);
	rigidbody->CollidesWithTilemap(false);
}

void CameraMovement::Values::Terminate()
{
	delete rigidbody;
	delete sprite;
}
