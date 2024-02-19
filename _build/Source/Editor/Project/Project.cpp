#include "pch.h"
#include "Project.h"

void Project::SetTile(Vector2Int position, const TileData& data)
{
	if (isSaved)
	{
		isSaved = false;
		SetWindowTitle((GetWindowTitle() + '*').c_str());
	}
		
	tiles[position].SetData(data);
}

void Project::RemoveTile(Vector2Int position)
{
	tiles.erase(position);
}

void Project::Draw(Vector2Int begin, Vector2Int end, const std::function<Vector2Int(Vector2Int)>& gridToScreen)
{
	const auto endIter = tiles.end();

	for (int y = begin.y; y < end.y; ++y)
	{
		for (int x = begin.x; x < end.x; ++x)
		{
			Vector2Int position(x, y);
			auto iter = tiles.find(position);

			if (iter == endIter) continue;

			const TileData& tileData = iter->second.GetData();
			position = gridToScreen(position);
			const auto& data = fileData.at(tileData.pathHash);
			Rectangle source(tileData.imagePosition.x * tileSize.x, tileData.imagePosition.y * tileSize.y, tileSize.x, tileSize.y);
			DrawTextureRec(data.texture, source, position, tileData.tint);
		}
	}
}

vector<const FileData*> Project::GetFileData() const
{
	vector<const FileData*> value;
	for (const auto& data : fileData)
		value.push_back(&data.second);

	return value;
}

Project::Project(const fs::path& projectFile)
	: projectFile(projectFile)
	, isSaved(true)
{
	fstream inFile(projectFile);
	string line;
	if (!inFile.is_open())
		return;

	std::getline(inFile, line);
	tileSize = StringToVector(line);

	while (!inFile.eof())
	{
		std::getline(inFile, line);
		if (line.find("Path:") != string::npos)
		{
			fs::path imagePath = line.substr(5);
			FileData data(imagePath);
			fileData.insert({ data.hash, data });
			LoadTilesFromFile(data.hash, inFile);
		}
	}
}


void Project::Save()
{
	SaveAs(projectFile);
}

void Project::SaveAs(const fs::path& path)
{
	if (!fs::remove(path))
		return;

	unordered_map<size_t, vector<string>> data;

	for (auto& [hash, fData] : fileData)
		data.insert({ hash, {} });

	for (auto& [gridPosition, tile] : tiles)
		data.at(tile.GetData().pathHash).push_back(tile.GetSaveString(gridPosition));

	ofstream inFile(path);

	inFile << tileSize.x << ',' << tileSize.y << '\n';

	for (const auto& [pathHash, tileData] : data)
	{
		inFile << "Path:" << fileData.at(pathHash).filepath.string() << '\n';
		for (const auto& str : tileData)
			inFile << str << '\n';
		inFile << "--End\n";
	}

	inFile.close();
	isSaved = true;
	projectFile = path;

	SetWindowTitle(GetWindowTitle().c_str());
}

std::optional<TileData> Project::GetTile(Vector2Int gridPosition) const
{
	if (tiles.contains(gridPosition))
		return tiles.at(gridPosition).GetData();

	return std::nullopt;
}

void Project::LoadTilesFromFile(size_t hashValue, fstream& inFile)
{
	string line;

	while (!inFile.eof())
	{
		std::getline(inFile, line);
		if (line.find("--End") != string::npos)
			break;

		stringstream ss(line);

		std::getline(ss, line, '|');
		Vector2Int imagePosition = StringToVector(line);

		std::getline(ss, line, '|');
		Vector2Int gridPosition = StringToVector(line);

		Color color = WHITE;
		if (std::getline(ss, line) && !line.empty())
		{
			int value = stoi(line);
			color = IntToColor(value);
		}
			
		
		tiles.insert({ gridPosition, Tile(TileData(imagePosition, hashValue, color)) });
	}
}

Color Project::IntToColor(int value)
{
	Color color{};

	color.r = value >> 24;
	color.g = value >> 16;
	color.b = value >> 8;
	color.a = value;

	return color;
}

Vector2Int Project::StringToVector(const string& value)
{
	size_t pos = value.find(',');
	Vector2Int position;
	position.x = stoi(value.substr(0, pos));
	position.y = stoi(value.substr(pos + 1));
	return position;
}

FileData::FileData(const fs::path& filepath)
	: filepath(filepath)
{
	hash = fs::hash_value(filepath);
	name = filepath.stem().string();

	auto iter = textures.find(hash);

	if (iter == textures.end())
	{
		texture = LoadTexture(filepath.string().c_str());
		textures.insert({ hash, texture });
	}
	else
	{
		texture = iter->second;
	}
}

void FileData::Terminate()
{
	for (const auto& [hash, texture] : textures)
		UnloadTexture(texture);
}




