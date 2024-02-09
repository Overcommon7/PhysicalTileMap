#include "pch.h"
#include "TileSelector.h"

#include "Editor/Project/Project.h"

void TileSelector::SetProject(Project* project)
{
	this->project = project;
	if (project == nullptr)
		return;

	fileData = project->GetFileData();
	if (fileData.empty()) return;

	SelectNewFileData(fileData.front());
}

const TileData& TileSelector::GetSelectedTileData() const
{
	return currentTile;
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

	if (project == nullptr)
		return;

	DrawTabs();
}

void TileSelector::RaylibDraw()
{
	if (project == nullptr)
		return;

	const Vector2Int tileSize = project->GetTileSize();
	Vector2Int position = Vector2Int(spacing, spacing);
	const Vector2Int resolution(camera.GetResolution());

	bool isInTexture = IsInsideTexture(::GetMousePosition());
	bool mouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
	bool selectNewTile = isInTexture && mouseClicked;

	for (int y = 0; y < texture.height; y += tileSize.y)
	{
		for (int x = 0; x < texture.width; x += tileSize.x)
		{
			Rectangle source(x, y, tileSize.x, tileSize.y);
			DrawTextureRec(texture, source, position, WHITE);
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
				
	
			position.x += tileSize.x + spacing;
			if (position.x > resolution.x - (tileSize.x + spacing))
			{
				position.y += tileSize.y + spacing;
				position.x = spacing;
			}
		}
	}
}

void TileSelector::Update()
{
	if (project == nullptr)
		return;

	const Vector2Int resolution(camera.GetResolution());
	Vector2Int size(windowSize);
	size.x *= 0.9f;
	size.y *= 0.9f;

	if (size.x >= resolution.x * 1.50f ||
		size.y >= resolution.y * 1.50f || 
		size.x <= resolution.x * 0.75f || 
		size.y <= resolution.y * 0.75f)
	{
		const Vector2Int tileSize(project->GetTileSize());
		size.x -= size.x % tileSize.x;
		size.x -= size.y % tileSize.y;
		camera.SetResolution(size);
	}

	newDataSelected = false;
}

void TileSelector::SelectNewFileData(const FileData* data)
{
	currentTile.pathHash = data->hash;
	currentTile.imagePosition = Vector2Int();
	currentTile.tint = WHITE;

	texture = data->texture;

	OnDataChanged.Invoke(currentTile);
}

void TileSelector::SelectNewTileData(Vector2Int imagePosition)
{
	currentTile.tint = WHITE;
	currentTile.imagePosition = imagePosition;
	OnDataChanged.Invoke(currentTile);
}

void TileSelector::DrawTabs()
{
	if (!ImGui::BeginTabBar("Tile Images"))
		return;

	for (const auto& data : fileData)
	{
		if (!ImGui::BeginTabItem(data->name.c_str())) continue;

		if (data->hash != currentTile.pathHash)
		{
			SelectNewFileData(data);
		}

		ITextureWindow::ImGuiDraw();
		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();
}
