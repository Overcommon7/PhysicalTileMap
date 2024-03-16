#pragma once
struct TileData
{
	TileData() = default;
	TileData(Vector2Int imagePosition, size_t pathHash, Color tint, bool isDeath = false)
		: mImagePosition(imagePosition), mPathHash(pathHash), mTint(tint), mIsDeath(isDeath) {}

	TileData(const TileData& data)
		: TileData(data.mImagePosition, data.mPathHash, data.mTint, data.mIsDeath) {}

	Color mTint = WHITE;
	Vector2Int mImagePosition{};
	size_t mPathHash{};
	bool mIsDeath = false;
};

class Tile
{
public:
	const TileData& GetData() const { return mData; }
	string GetSaveString(Vector2Int gridPosition);
	void SetData(const TileData& data);

	Tile(const TileData& data);
	Tile() = default;
private:
	TileData mData;
	int ColorToInt(Color color);
};


/* File Structure
* 1,4|4,5|1943924
* imagePosition
* gridPosition
* packedColor
*/

