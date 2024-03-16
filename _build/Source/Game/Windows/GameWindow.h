#pragma once
#include "ImGuiWindow/ITextureWindow.h"
#include "Game/Camera/CameraMovement.h"

class Project;
class Player;
class Enemy;

class GameWindow : public ITextureWindow
{
	struct EnemySpawnValues
	{
		Vector2Int spawnPosition;
	};
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
	vector<Enemy*>* GetEnemies() { return &mEnemies; }
private:
	Project* mProject;
	Player* mPlayer;
	vector<Enemy*> mEnemies;
	Vector2Int mStart;
	Vector2Int mEnd;
	CameraMovement::Values mCameraValues;
	function<void(const float)> mFixedUpdate{};
	EnemySpawnValues mSpawnValues;


	void DrawFileMenu();
	void DrawEditMenu();
	void DrawDebugMenu();
	void DrawEnemySpawnMenu();

	void UpdateStartAndEnd();
};

