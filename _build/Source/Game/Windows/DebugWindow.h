#pragma once
#include "ImGuiWindow/IWindow.h"
#include "Game/Camera/CameraMovement.h"


class Player;
namespace Render
{
	class Camera;
}


class DebugWindow : public IWindow    
{
public:
	IWindowConstructor(DebugWindow) {}
	~DebugWindow() { mPlayer = nullptr; }
	void SetPlayer(Player* player) { mPlayer = player; }
	void SetCameraValues(CameraMovement::Values* values) { mCameraValues = values; }
	void SetCamera(Render::Camera* camera) { mCamera = camera; }
	void ImGuiDraw() override;

	
private:
	Player* mPlayer = nullptr;
	CameraMovement::Values* mCameraValues = nullptr;
	Render::Camera* mCamera = nullptr;
};

