#pragma once
#include "ImGuiWindow/IWindow.h"
#include "Game/Camera/CameraMovement.h"


class Player;
class Enemy;
namespace Render
{
	class Camera;
}


class DebugWindow : public IWindow    
{
public:
	IWindowConstructor(DebugWindow) {}
	~DebugWindow();
	void SetPlayer(Player* player) { mPlayer = player; }
	void SetCameraValues(CameraMovement::Values* values) { mCameraValues = values; }
	void SetCamera(Render::Camera* camera) { mCamera = camera; }
	void SetEnemiesVector(vector<Enemy*>* enemies) { mEnemies = enemies; }
	void ImGuiDraw() override;

	
private:
	Player* mPlayer = nullptr;
	CameraMovement::Values* mCameraValues = nullptr;
	vector<Enemy*>* mEnemies = nullptr;
	Render::Camera* mCamera = nullptr;
};

