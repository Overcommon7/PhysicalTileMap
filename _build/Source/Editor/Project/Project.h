#pragma once
#include "Tile.h"

class SceneView;

struct FileData
{
	FileData(const fs::path& filepath);
	static void Terminate();

	fs::path filepath;
	string name;
	size_t hash;
	Texture2D texture;
private:
	bool loadedTexture;
	
	inline static unordered_map<size_t, Texture2D> textures;
	
};

class Project
{
public:
	Vector2Int GetTileSize() const { return tileSize; }

	void SetTile(Vector2Int position, const TileData& data);
	void RemoveTile(Vector2Int position);
	void Draw(Vector2Int begin, Vector2Int end, const std::function<Vector2Int(Vector2Int)>& gridToScreen);

	vector<const FileData*> GetFileData() const;
	Project(const fs::path& projectFile);
	void Save();
	void SaveAs(const fs::path& path);
	void Clear() { tiles.clear(); }

	std::optional<TileData> GetTile(Vector2Int gridPosition) const;

	string GetWindowTitle() const { return "OverTiled - " + projectFile.stem().string(); }
private:
		
	unordered_map<Vector2Int, Tile, Vector2IntHash> tiles;
	unordered_map<size_t, FileData> fileData;
	fs::path projectFile;
	bool isSaved;
	Vector2Int tileSize;

	void LoadTilesFromFile(size_t hashValue, fstream& inFile);
	Color IntToColor(int value);
	Vector2Int StringToVector(const string& value);

	
};



