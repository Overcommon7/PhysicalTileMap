#include "pch.h"
#include "TileSelector.h"

#include "Editor/Project/Project.h"

void TileSelector::SetProject(Project* project)
{
	this->mProject = project;
	if (project == nullptr)
		return;

	mFileData = project->GetFileData();
	if (mFileData.empty()) return;

	SelectNewFileData(mFileData.front());
}

const TileData& TileSelector::GetSelectedTileData() const
{
	return mCurrentTile;
}

void TileSelector::SelectNewTile(const TileData& data)
{
	auto it = std::find_if(mFileData.begin(), mFileData.end(), [&data](const FileData* fData) {
		return fData->mHash == data.pathHash;
		});

	if (it == mFileData.end())
		return;

	mTexture = (*it)->mTexture;
	mCurrentTile = data;
	
	mNewTileSelected = true;
	mNewDataSelected = true;

	OnDataChanged.Invoke(mCurrentTile);
}

const Texture2D& TileSelector::GetCurrentTexture()
{
	return mTexture;
}

void TileSelector::ImGuiDraw()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Debug##1"))
		{
			ITextureWindow::DrawDebugMenuItems();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (mProject == nullptr)
		return;

	DrawTabs();
}

void TileSelector::RaylibDraw()
{
	if (mProject == nullptr)
		return;

	DrawProject();
	DrawMiniTile();
}

void TileSelector::Update()
{
	if (mProject == nullptr)
		return;

	const Vector2Int resolution(camera.GetResolution());
	Vector2Int size(mWindowSize);
	size.x *= 0.9f;
	size.y *= 0.9f;

	if (size.x >= resolution.x * 1.50f ||
		size.y >= resolution.y * 1.50f || 
		size.x <= resolution.x * 0.75f || 
		size.y <= resolution.y * 0.75f)
	{
		const Vector2Int tileSize(mProject->GetTileSize());
		size.x -= size.x % tileSize.x;
		size.x -= size.y % tileSize.y;
		camera.SetResolution(size);
	}

	mNewDataSelected = false;
}

void TileSelector::DrawProject()
{
	const Vector2Int tileSize = mProject->GetTileSize();
	Vector2Int position = Vector2Int(sSpacing, sSpacing);
	const Vector2Int resolution(camera.GetResolution());

	bool isInTexture = IsInsideTexture(::GetMousePosition());
	bool mouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
	bool selectNewTile = isInTexture && mouseClicked;

	for (int y = 0; y < mTexture.height; y += tileSize.y)
	{
		for (int x = 0; x < mTexture.width; x += tileSize.x)
		{
			Rectangle source(x, y, tileSize.x, tileSize.y);
			DrawTextureRec(mTexture, source, position, WHITE);
			bool isColliding = false;

			Rectangle collider(position.x, position.y, tileSize.x, tileSize.y);
			if (CheckCollisionPointRec(localMousePosition, collider))
			{
				DrawRectangleRec(collider, { 120, 120, 120, 120 });
				isColliding = true;
			}

			if (selectNewTile && isColliding)
			{
				Vector2Int imagePosition(x, y);
				SelectNewTileData(imagePosition / tileSize);					
			}


			position.x += tileSize.x + sSpacing;
			if (position.x > resolution.x - (tileSize.x + sSpacing))
			{
				position.y += tileSize.y + sSpacing;
				position.x = sSpacing;
			}
		}
	}
}

void TileSelector::DrawMiniTile()
{
	const auto position(this->GetMousePosition());
	const auto tileSize(mProject->GetTileSize());
	const auto texturePosition = mCurrentTile.imagePosition * tileSize;

	const Rectangle source(texturePosition.x, texturePosition.y, tileSize.x, tileSize.y);
	const Rectangle dest(position.x - 15.f, position.y - 15.f, 25.f, 25.f);
	const Vector2 origin = { 0.0f, 0.0f };

	DrawTexturePro(mTexture, source, dest, origin, 0.f, mCurrentTile.tint);
}

void TileSelector::SelectNewFileData(const FileData* data)
{
	mCurrentTile.pathHash = data->mHash;
	mCurrentTile.imagePosition = Vector2Int();
	mCurrentTile.tint = WHITE;

	mTexture = data->mTexture;
	mNewDataSelected = true;

	OnDataChanged.Invoke(mCurrentTile);
}

void TileSelector::SelectNewTileData(Vector2Int imagePosition)
{
	mCurrentTile.tint = WHITE;
	mCurrentTile.imagePosition = imagePosition;
	OnDataChanged.Invoke(mCurrentTile);
}

void TileSelector::DrawTabs()
{
	if (!ImGui::BeginTabBar("Tile Images"))
		return;

		

	for (const auto& data : mFileData)
	{
		if (mNewTileSelected)
		{
			bool isOpen = data->mHash == mCurrentTile.pathHash;
			if (!ImGui::BeginTabItem(data->mName.c_str(), &isOpen))
				continue;
			
			ITextureWindow::ImGuiDraw();
			ImGui::EndTabItem();
		}
		else
		{
			if (!ImGui::BeginTabItem(data->mName.c_str())) continue;

			if (data->mHash != mCurrentTile.pathHash)
			{
				SelectNewFileData(data);
			}

			ITextureWindow::ImGuiDraw();
			ImGui::EndTabItem();
		}	
	}

	mNewTileSelected = false;
	ImGui::EndTabBar();
}
