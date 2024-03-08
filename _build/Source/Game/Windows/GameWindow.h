#pragma once
#include "ImGuiWindow/ITextureWindow.h"
#include "Game/Camera/CameraMovement.h"

class Project;
class Player;

class GameWindow : public ITextureWindow
{
public:
	ITextureWindowConstructorHeader(GameWindow);

	void Start(Project* project);
	void Stop();

	void Update() override;
	void RaylibDraw() override;
	void ImGuiDraw() override;

	Vector2Int ScreenToGrid(Vector2Int screenPosition) const;
	Vector2Int GridToScreen(Vector2Int gridPosition) const;

	Player* GetPlayer() { return mPlayer; }
	CameraMovement::Values* GetCameraValues() { return &mCameraValues; }
private:
	Project* mProject;
	Player* mPlayer;
	Vector2Int mStart;
	Vector2Int mEnd;
	CameraMovement::Values mCameraValues;
	function<void(const float)> mFixedUpdate{};

	void DrawFileMenu();
	void DrawEditMenu();
	void DrawDebugMenu();

	void UpdateStartAndEnd();
};

