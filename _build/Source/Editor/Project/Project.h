#pragma once
#include "Tile.h"

class SceneView;

struct FileData
{
	FileData(const fs::path& filepath);
	static void Terminate();

	fs::path mFilepath;
	string mName;
	size_t mHash;
	Texture2D mTexture;
private:
	bool mLoadedTexture;
	
	inline static unordered_map<size_t, Texture2D> sTextures;
	
};

class Project
{
public:
	enum class DrawMode
	{
		Editor,
		Game
	};
public:
	Vector2Int GetTileSize() const { return mTileSize; }
	Vector2Int GetPlayerStartPosition() const { return mStartPosition; }
	
	void SetPlayerSpawnPosition(Vector2Int spawnPosition) { mStartPosition = spawnPosition; }
	void SetTile(Vector2Int position, const TileData& data);
	void RemoveTile(Vector2Int position);
	void Draw(Vector2Int begin, Vector2Int end, const std::function<Vector2Int(Vector2Int)>& gridToScreen, DrawMode mode);

	vector<const FileData*> GetFileData() const;
	Project(const fs::path& projectFile);
	~Project();
	
	void Save();
	void SaveAs(const fs::path& path);
	void Clear() { mTiles.clear(); }

	std::optional<TileData> GetTile(Vector2Int gridPosition) const;

	const fs::path& GetSavePath() const { return mProjectFile; }
	string GetWindowTitle() const { return "OverTiled - " + mProjectFile.stem().string(); }
private:
		
	unordered_map<Vector2Int, Tile, Vector2IntHash> mTiles;
	unordered_map<size_t, FileData> mFileData;
	fs::path mProjectFile;
	bool mIsSaved;
	Vector2Int mTileSize;
	Vector2Int mStartPosition;

	void LoadTilesFromFile(size_t hashValue, fstream& inFile);
	Color IntToColor(int value);
	Vector2Int StringToVector(const string& value);

	void Load();
};



