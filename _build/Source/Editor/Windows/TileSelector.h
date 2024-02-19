#pragma once
#include "ImGuiWindow/ITextureWindow.h"
#include "../Project/Tile.h"

class Project;
struct FileData;


class TileSelector : public ITextureWindow
{
public:
	ITextureWindowConstructor(TileSelector) {
		hasMenuBar = true;
	}
	void SetProject(Project* project);

	const TileData& GetSelectedTileData() const;
	bool IsNewDataSelected() const { return newDataSelected; }
	
	void SelectNewTile(const TileData& data);
	const Texture2D& GetCurrentTexture();

	Action<TileData> OnDataChanged;
private:
	Project* project;
	vector<const FileData*> fileData;

	TileData currentTile;
	Texture2D texture;
	bool newDataSelected = false;
	bool newTileSelected = false;

	void ImGuiDraw() override;
	void RaylibDraw() override;
	void Update() override;

	void DrawProject();
	void DrawMiniTile();

	void SelectNewTileData(Vector2Int imagePosition);
	void SelectNewFileData(const FileData* data);

	void DrawTabs();

	Vector2Int ScreenToGrid(Vector2Int screenPosition) const;

	const int spacing = 2;
};

