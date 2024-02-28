#pragma once
#include "ImGuiWindow/ITextureWindow.h"
#include "Editor/Project/Tile.h"

#include "Editor/Utilities/Selection.h"
#include "Editor/Utilities/EyeDropper.h"


class Project;
class TileSelector;


class SceneView final : public ITextureWindow   
{
public:
	ITextureWindowConstructor(SceneView) 
		, mProject(nullptr)
		, mTileSelector(nullptr)
	{
		mHasMenuBar = true;
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
	void DrawEditMenu();
	void DrawMiniTile();


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
		EyeDropper eyeDropper;
		Selection selection;
	};

	struct ImGuiValues
	{
		bool isHoveringMenu = false;
	};
private:

	Settings mSettings;
	ImGuiValues mImGuiValues;
	EditorValues mEditorValues;
	Project* mProject;
	Vector2Int mStart;
	Vector2Int mEnd;
	TileData mCurrentTileData;
	TileSelector* mTileSelector;
	std::function<Vector2Int(Vector2Int)> mGridToScreen;

	friend class Selection;
};

