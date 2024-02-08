#pragma once
#include "ImGuiWindow/ITextureWindow.h"
#include "../Project/Tile.h"

class Project;
struct FileData;


class TileSelector : public ITextureWindow
{
public:
	ITextureWindowConstructor(TileSelector) {}

	void SetProject(Project* project);

	const TileData& GetSelectedTileData() const;
	bool IsNewDataSelected() const { return newDataSelected; }

	Action<TileData> OnDataChanged;
private:
	Project* project;
	vector<const FileData*> fileData;

	TileData currentTile;
	Texture2D texture;
	bool newDataSelected = false;
	bool selectNewTile = false;
	Vector2Int selectionPosition{};

	void ImGuiDraw() override;
	void RaylibDraw() override;
	void Update() override;

	void SelectNewFileData(const FileData* data);
	void SelectNewTileData(Vector2Int imagePosition);

	void DrawTabs();
	void UpdateTileSelection();

	Vector2Int ScreenToGrid(Vector2Int screenPosition);

	const int spacing = 2;
};

