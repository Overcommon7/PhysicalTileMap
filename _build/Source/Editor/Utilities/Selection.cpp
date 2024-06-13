#include "pch.h"
#include "Selection.h"

#include "Editor/Project/Project.h"
#include "Editor/Windows/SceneView.h"
#include "Editor/Windows/TileSelector.h"

void Selection::BeginDragPoint(Vector2Int gridPosition)
{
	selectionStart = gridPosition;
	hasSelection = true;
}

void Selection::UpdateEndPoint(Vector2Int endPoint)
{
	Vector2Int start; 
	Vector2Int end;

	if (endPoint.x < selectionStart.x)
	{
		start.x = endPoint.x;
		end.x = selectionStart.x;
	}
	else
	{
		start.x = selectionStart.x;
		end.x = endPoint.x;
	}

	if (endPoint.y < selectionStart.y)
	{
		start.y = endPoint.y;
		end.y = selectionStart.y;
	}
	else
	{
		start.y = selectionStart.y;
		end.y = endPoint.y;
	}
		
	selectionEnd = end;
	selectionStart = start;
}

void Selection::Update(SceneView* const sceneView, const TileSelector* const tileSelector)
{
	if (ImGui::IsKeyPressed(ImGuiKey_Escape))
		CancelSelection();

	if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
	{
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			Vector2Int gridPosition(sceneView->ScreenToGrid(sceneView->GetMousePosition(), true));
			BeginDragPoint(gridPosition);
		}

		if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
		{
			if (hasSelection)
			{
				Vector2Int gridPosition(sceneView->ScreenToGrid(sceneView->GetMousePosition(), true));
				UpdateEndPoint(gridPosition);
			}	
		}
	}

	if (hasSelection)
	{
		if (ImGui::IsKeyPressed(ImGuiKey_Delete))
			DeleteTilesFromSelection(sceneView->mProject);

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			Vector2 mousePosition(::GetMousePosition());
			if (tileSelector->IsInsideTexture(mousePosition))
				return;

			if (sceneView->IsInsideTexture(mousePosition))
			{
				if (CheckCollisionPointRec(sceneView->GetMousePosition(), GetSelectionScreenRec(sceneView->mGridToScreen)))
				{
					FillSelectedTiles(sceneView->mProject, sceneView->mCurrentTileData);
				}
				else
				{
					CancelSelection();
				}
			}
		}
	}

	
}

void Selection::RaylibDraw(SceneView* const sceneView)
{
	if (!hasSelection)
		return;

	DrawRectangleLinesEx(GetSelectionScreenRec(sceneView->mGridToScreen), sceneView->mSettings.gridThickness, RED);
}

void Selection::DeleteTilesFromSelection(Project* project)
{
	for (int y = selectionStart.y; y < selectionEnd.y; ++y)
	{
		for (int x = selectionStart.x; x < selectionEnd.x; ++x)
		{
			project->RemoveTile(Vector2Int(x, y));
		}
	}
}

void Selection::FillSelectedTiles(Project* project, const TileData& tileData)
{
	for (int y = selectionStart.y; y < selectionEnd.y; ++y)
	{
		for (int x = selectionStart.x; x < selectionEnd.x; ++x)
		{
			project->SetTile(Vector2Int(x, y), tileData);
		}
	}
}

void Selection::CancelSelection()
{
	selectionEnd = Vector2Int(0, 0);
	selectionStart = Vector2Int(0, 0);
	hasSelection = false;
}

void Selection::OnNewDataSelected(Project* project, const TileData& data)
{
	if (!hasSelection)
		return;

	FillSelectedTiles(project, data);
}

Rectangle Selection::GetSelectionScreenRec(const std::function<Vector2Int(Vector2Int)> gridToScreen) const
{
	auto start(gridToScreen(selectionStart));
	auto end(gridToScreen(selectionEnd));
	Rectangle selectionRec;
	selectionRec.x = start.x;
	selectionRec.y = start.y;
	selectionRec.width = end.x - start.x;
	selectionRec.height = end.y - start.y;
	return selectionRec;
}
