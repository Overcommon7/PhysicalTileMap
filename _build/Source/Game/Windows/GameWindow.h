#pragma once
#include "ImGuiWindow/ITextureWindow.h"

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
private:
	Project* mProject;
	Player* mPlayer;
	Vector2Int mStart;
	Vector2Int mEnd;

	void DrawFileMenu();
	void DrawEditMenu();
	void DrawDebugMenu();

	void UpdateStartAndEnd();
};

