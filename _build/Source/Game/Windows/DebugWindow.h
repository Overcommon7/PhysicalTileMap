#pragma once
#include "ImGuiWindow/IWindow.h"
#include "Game/Camera/CameraMovement.h"


class Player;


class DebugWindow : public IWindow    
{
public:
	IWindowConstructor(DebugWindow) {}
	~DebugWindow() { mPlayer = nullptr; }
	void SetPlayer(Player* player) { mPlayer = player; }
	void SetCamera(CameraMovement::Values* values) { mCameraValues = values; }
	void ImGuiDraw() override;

	
private:
	Player* mPlayer = nullptr;
	CameraMovement::Values* mCameraValues = nullptr;
};

