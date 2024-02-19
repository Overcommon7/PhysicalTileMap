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

private:
	Project* project;
	Player* player;
	Vector2Int start;
	Vector2Int end;

	void DrawFileMenu();
	void DrawEditMenu();
	void DrawDebugMenu();

	Vector2Int ScreenToGrid(Vector2Int screenPosition) const;
	Vector2Int GridToScreen(Vector2Int gridPosition) const;

	void UpdateStartAndEnd();
};

