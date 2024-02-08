#include "pch.h"
#include "Tile.h"

string Tile::GetSaveString(Vector2Int gridPosition)
{
	stringstream ss;
	ss << data.imagePosition.x << ',' << data.imagePosition.y << '|';
	ss << gridPosition.x << ',' << gridPosition.y << '|';
	ss << ColorToInt(data.tint);

	return ss.str();
}

Tile::Tile(const TileData& data)
    :data(data)
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

