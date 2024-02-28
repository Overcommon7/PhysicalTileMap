#pragma once
#include "ImGuiWindow/ITextureWindow.h"
#include "../Project/Tile.h"

class Project;
struct FileData;


class TileSelector : public ITextureWindow
{
public:
	ITextureWindowConstructor(TileSelector)
		, mTexture({})
		, mProject(nullptr)
	{
		mHasMenuBar = true;
		
		
	}
	void SetProject(Project* project);

	const TileData& GetSelectedTileData() const;
	bool IsNewDataSelected() const { return mNewDataSelected; }
	
	void SelectNewTile(const TileData& data);
	const Texture2D& GetCurrentTexture();

	Action<TileData> OnDataChanged;
private:
	Project* mProject;
	vector<const FileData*> mFileData;

	TileData mCurrentTile;
	Texture2D mTexture;
	bool mNewDataSelected = false;
	bool mNewTileSelected = false;

	void ImGuiDraw() override;
	void RaylibDraw() override;
	void Update() override;

	void DrawProject();
	void DrawMiniTile();

	void SelectNewTileData(Vector2Int imagePosition);
	void SelectNewFileData(const FileData* data);

	void DrawTabs();

	Vector2Int ScreenToGrid(Vector2Int screenPosition) const;

	const static int sSpacing = 2;
};

