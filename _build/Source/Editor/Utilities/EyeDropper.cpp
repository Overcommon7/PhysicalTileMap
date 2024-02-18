#include "pch.h"
#include "EyeDropper.h"

#include "Editor/Windows/TileSelector.h"
#include "Editor/Project/Project.h"

void EyeDropper::Select(Project* project, TileSelector* tileSelector, Vector2Int mouseGridPosition)
{
	auto tileData = project->GetTile(mouseGridPosition);
	if (tileData)
	{	
		tileSelector->SelectNewTile(*tileData);
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
