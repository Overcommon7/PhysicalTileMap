#pragma once
#include "ImGuiWindow/ITextureWindow.h"
#include "../Project/Tile.h"

class Project;


class SceneView final : public ITextureWindow   
{
public:
	ITextureWindowConstructor(SceneView) {}

	void SetProject(Project* project);
	void SetNewTileData(const TileData& data);
private:
	void RaylibDraw() override;
	void ImGuiDraw() override;
	void ImGuiDrawBegin() override;
	void Update() override;


	void UpdateCamera();
	void DrawGrid();
	void UpdateStartAndEnd();
	void UpdateProject();
	void DrawSettingsMenuItem();
	void DrawDebugMenuItem();
	void DrawFileMenu();

	Vector2Int ScreenToGrid(Vector2Int screenPosition, bool isTexturePosition = false);
	Vector2Int GridToScreen(Vector2Int gridPosition);


private:
	struct Settings
	{
		float zoomSpeed = 5;
		float moveSpeed = 1;
		int gridThickness = 2;
		bool showGrid = true;
		Color gridColor = RAYWHITE;
	};

	struct ImGuiValues
	{
		bool isHoveringMenu = false;
	};
private:
	Settings settings;
	ImGuiValues imGuiValues;
	Project* project;
	Vector2Int start;
	Vector2Int end;
	TileData currentTileData;
};

