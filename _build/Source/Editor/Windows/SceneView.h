#pragma once
#include "ImGuiWindow/ITextureWindow.h"
#include "../Project/Tile.h"
#include "Editor/Utilities/Selection.h"

class Project;
class TileSelector;


class SceneView final : public ITextureWindow   
{
public:
	ITextureWindowConstructor(SceneView) 
	{
		hasMenuBar = true;
	}

	void SetProject(Project* project);
	void SetNewTileData(const TileData& data);
	void SetTileSelector(TileSelector* const tileSelector);
	void Initialize() override;
private:
	void RaylibDraw() override;
	void ImGuiDraw() override;
	void Update() override;


	void UpdateInputs();
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

	struct EditorValues
	{
		bool usingEyeDropper = false;
		Selection selection;
	};

	struct ImGuiValues
	{
		bool isHoveringMenu = false;
	};
private:
	Settings settings;
	ImGuiValues imGuiValues;
	EditorValues editorValues;
	Project* project;
	Vector2Int start;
	Vector2Int end;
	TileData currentTileData;
	const TileSelector* tileSelector;
	std::function<Vector2Int(Vector2Int)> gridToScreen;

	friend class Selection;
};

