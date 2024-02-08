#pragma once
struct TileData
{
	TileData() = default;
	TileData(Vector2Int imagePosition, size_t pathHash, Color tint)
		: imagePosition(imagePosition), pathHash(pathHash), tint(tint) {}

	Color tint = WHITE;
	Vector2Int imagePosition{};
	size_t pathHash{};
};

class Tile
{
public:
	const TileData& GetData() const { return data; }
	string GetSaveString(Vector2Int gridPosition);
	Tile(const TileData& data);
	Tile() = default;
private:
	TileData data;

	int ColorToInt(Color color);
};


/* File Structure
* 1,4|4,5|1943924
* imagePosition
* gridPosition
* packedColor
*/
