#include "pch.h"
#include "Tile.h"

string Tile::GetSaveString(Vector2Int gridPosition)
{
	stringstream ss;
	ss << mData.mImagePosition.x << ',' << mData.mImagePosition.y << '|';
	ss << gridPosition.x << ',' << gridPosition.y << '|';
	ss << ColorToInt(mData.mTint) << '|';
	ss << mData.mIsDeath ? 1 : 0;

	return ss.str();
}

void Tile::SetData(const TileData& data)
{
	this->mData = data;
}


Tile::Tile(const TileData& data)
    :mData(data)
{

}


int Tile::ColorToInt(Color color)
{
	int result = 0;
	result |= color.r;
	result <<= 8;
	result |= color.g;
	result <<= 8;
	result |= color.b;
	result <<= 8;
	result |= color.a;
	return result;
}

