#include "pch.h"
#include "Project.h"

void Project::SetTile(Vector2Int position, const TileData& data)
{
	if (mIsSaved)
	{
		mIsSaved = false;
		SetWindowTitle((GetWindowTitle() + '*').c_str());
	}
		
	mTiles[position].SetData(data);
}

void Project::RemoveTile(Vector2Int position)
{
	mTiles.erase(position);
}

void Project::Draw(Vector2Int begin, Vector2Int end, const std::function<Vector2Int(Vector2Int)>& gridToScreen, DrawMode mode)
{
	const auto endIter = mTiles.end();

	for (int y = begin.y; y < end.y; ++y)
	{
		for (int x = begin.x; x < end.x; ++x)
		{
			Vector2Int position(x, y);
			auto iter = mTiles.find(position);

			if (iter == endIter) continue;

			const TileData& tileData = iter->second.GetData();
			position = gridToScreen(position);
			const auto& data = mFileData.at(tileData.mPathHash);
			Rectangle source(tileData.mImagePosition.x * mTileSize.x, tileData.mImagePosition.y * mTileSize.y, mTileSize.x, mTileSize.y);
			DrawTextureRec(data.mTexture, source, position, tileData.mTint);
			if (tileData.mIsDeath && mode == DrawMode::Editor)
				DrawRectangle(position.x, position.y, mTileSize.x, mTileSize.y, { 230, 41, 55, 100 });
		}
	}
}

vector<const FileData*> Project::GetFileData() const
{
	vector<const FileData*> value;
	for (const auto& data : mFileData)
		value.push_back(&data.second);

	return value;
}

Project::Project(const fs::path& projectFile)
	: mProjectFile(projectFile)
	, mIsSaved(true)
{
	Load();
}

void Project::Load()
{
	fstream inFile(mProjectFile);
	string line;
	if (!inFile.is_open())
		return;

	std::getline(inFile, line);
	mTileSize = StringToVector(line);

	std::getline(inFile, line);
	mStartPosition = StringToVector(line);

	while (!inFile.eof())
	{
		std::getline(inFile, line);
		if (line.find("Path:") != string::npos)
		{
			fs::path imagePath = line.substr(5);
			FileData data(imagePath);
			mFileData.insert({ data.mHash, data });
			LoadTilesFromFile(data.mHash, inFile);
		}
	}
}

Project::~Project()
{

}


void Project::Save()
{
	SaveAs(mProjectFile);
}

void Project::SaveAs(const fs::path& path)
{
	fs::remove(path);

	bool newLocation = path != mProjectFile;
	fs::path newDir = std::filesystem::absolute(path).parent_path();
	

	unordered_map<size_t, vector<string>> data;

	for (auto& [hash, fData] : mFileData)
		data.insert({ hash, {} });

	for (auto& [gridPosition, tile] : mTiles)
		data.at(tile.GetData().mPathHash).push_back(tile.GetSaveString(gridPosition));

	ofstream inFile(path);

	inFile << mTileSize.x << ',' << mTileSize.y << '\n';
	inFile << mStartPosition.x << ',' << mStartPosition.y << '\n';

	for (const auto& [pathHash, tileData] : data)
	{
		if (newLocation)
		{
			const auto& from(mFileData.at(pathHash).mFilepath);
			const auto dest(newDir / (from.stem().string() + from.extension().string()));
			inFile << "Path:" << dest.string() << '\n';
			if (!fs::exists(dest))
				fs::copy_file(from, dest);
		}
		else
		{
			inFile << "Path:" << fs::relative(mFileData.at(pathHash).mFilepath).string() << '\n';
		}
		
		for (const auto& str : tileData)
			inFile << str << '\n';
		inFile << "--End\n";
	}

	inFile.close();
	mIsSaved = true;
	mProjectFile = path;
	if (mProjectFile.extension() != ".otp")
	{
		fs::path old(mProjectFile);
		fs::rename(old, mProjectFile.replace_extension(".otp"));
	}
	

	SetWindowTitle(GetWindowTitle().c_str());

	if (newLocation)
	{
		Load();
	}
}

std::optional<TileData> Project::GetTile(Vector2Int gridPosition) const
{
	if (mTiles.contains(gridPosition))
		return mTiles.at(gridPosition).GetData();

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
		if (std::getline(ss, line, '|') && !line.empty())
		{
			int value = stoi(line);
			color = IntToColor(value);
		}
		
		bool isDeath = false;
		if (std::getline(ss, line, '|') && !line.empty())
		{
			isDeath = line.find('1') != string::npos;
		}

		
		mTiles.insert({ gridPosition, Tile(TileData(imagePosition, hashValue, color, isDeath)) });
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
	: mFilepath(filepath)
{
	mHash = fs::hash_value(filepath);
	mName = filepath.stem().string();

	auto iter = sTextures.find(mHash);

	if (iter == sTextures.end())
	{
		mTexture = LoadTexture(filepath.string().c_str());
		sTextures.insert({ mHash, mTexture });
	}
	else
	{
		mTexture = iter->second;
	}
}

void FileData::Terminate()
{
	for (const auto& [hash, texture] : sTextures)
		UnloadTexture(texture);
}




