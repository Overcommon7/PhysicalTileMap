#include "pch.h"
#include "EyeDropper.h"

#include "Editor/Windows/TileSelector.h"
#include "Editor/Project/Project.h"

void EyeDropper::Select(Project* project, TileSelector* tileSelector, Vector2Int mouseGridPosition)
{
	auto tile = project->GetTile(mouseGridPosition);
	if (tile.has_value())
	{	
		auto files = project->GetFileData();
		size_t pathHash = tile->pathHash;
		auto it = std::find_if(files.begin(), files.end(), [pathHash](const FileData* data) {
			return data->hash == pathHash;
			});

		tileSelector->SelectNewFileData(*it);
		tileSelector->SelectNewTileData(tile->imagePosition);
	}

	Cancel();
}

void EyeDropper::Update()
{
	if (ImGui::IsKeyPressed(ImGuiKey_I))
		Activate();

	if (ImGui::IsKeyPressed(ImGuiKey_Escape))
		Cancel();
}

void EyeDropper::Activate()
{
	mIsActive = true;
}

void EyeDropper::Cancel()
{
    mIsActive = false;
}

bool EyeDropper::IsActive()
{
    return mIsActive;
}
