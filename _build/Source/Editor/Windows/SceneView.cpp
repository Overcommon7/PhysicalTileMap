#include "pch.h"
#include "SceneView.h"

#include "../Project/Project.h"
#include "TileSelector.h"
#include "App/App.h"
#include "../Editor.h"

#include "ImGuiUtilities/ImGuiUtils.h"


void SceneView::SetProject(Project* project)
{
	this->mProject = project;
	SetWindowTitle(project->GetWindowTitle().c_str());
}

void SceneView::SetNewTileData(const TileData& data)
{
	mCurrentTileData = data;
	mEditorValues.selection.OnNewDataSelected(mProject, data);
}

void SceneView::SetTileSelector(TileSelector* const tileSelector)
{
	this->mTileSelector = tileSelector;
}

void SceneView::Initialize()
{
	auto GridToScreen = [this](Vector2Int grid) -> Vector2Int
		{
			return this->GridToScreen(grid);
		};

	mGridToScreen = GridToScreen;
}

void SceneView::RaylibDraw()
{
	if (mProject == nullptr)
		return;

	if (mSettings.showGrid)
		DrawGrid();

	
	mProject->Draw(ScreenToGrid(mStart, true), ScreenToGrid(mEnd, true), mGridToScreen);
	mEditorValues.selection.RaylibDraw(this);
	DrawMiniTile();

	
}

void SceneView::ImGuiDraw()
{
	if (ImGui::BeginMenuBar())
	{
		mImGuiValues.isHoveringMenu = false;
		DrawFileMenu();
		DrawEditMenu();
		DrawSettingsMenuItem();
		DrawPlayerMenu();
		DrawDebugMenuItem();
		ImGui::EndMenuBar();
	}

	if (mProject == nullptr)
		return;

	UpdateCamera();

	if (!mImGuiValues.isHoveringMenu && !mEditorValues.eyeDropper.IsActive())
		UpdateProject();

	UpdateInputs();
	Vector2Int gridPosition(ScreenToGrid(this->GetMousePosition(), true));
	ImGui::Text("Mouse Grid Position: %i, %i", gridPosition.x, gridPosition.y);
	ITextureWindow::ImGuiDraw();
}

void SceneView::Update()
{
	if (mProject == nullptr)
		return;

	UpdateStartAndEnd();
}

void SceneView::UpdateInputs()
{
	mEditorValues.selection.Update(this, mTileSelector);
	mEditorValues.eyeDropper.Update();

	if (mEditorValues.eyeDropper.IsActive() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		mEditorValues.eyeDropper.Select(mProject, mTileSelector, ScreenToGrid(this->GetMousePosition(), true));
}

void SceneView::UpdateCamera()
{
	Camera2D& rayCamera = mCamera.GetRaylibCamera();

	if (ImGui::IsWindowHovered())
	{
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
		{
			Vector2Int vec(ImGui::GetMouseDragDelta(ImGuiMouseButton_Middle));
			ImGui::ResetMouseDragDelta(ImGuiMouseButton_Middle);
			rayCamera.target = Vector2Add(rayCamera.target, (Vector2Scale(vec, mSettings.moveSpeed * (-1 / rayCamera.zoom))));
		}

		auto wheel = GetMouseWheelMoveV().y;

		if (wheel != 0)
		{
			rayCamera.zoom += (wheel * Time::DeltaTime() * mSettings.zoomSpeed);
			rayCamera.zoom = std::clamp(rayCamera.zoom, 0.5f, 5.f);
		}
	}
}

void SceneView::DrawGrid()
{
	const Camera2D& rayCamera = mCamera.GetRaylibCamera();
	const int thickness = mSettings.showGrid ? mSettings.gridThickness : 0;

	const Vector2Int tileSize(mProject->GetTileSize() + thickness);	
	const Vector2Int dimensions(mEnd - mStart + tileSize);

	for (int x = mStart.x - (mStart.x % tileSize.x) ; x < mEnd.x; x += tileSize.x)
	{
		DrawRectangle(x, mStart.y, thickness, dimensions.y, mSettings.gridColor);
	}

	for (int y = mStart.y - (mStart.y % tileSize.y) ; y < mEnd.y; y += tileSize.y)
	{
		DrawRectangle(mStart.x, y, dimensions.x, thickness, mSettings.gridColor);
	}
}

void SceneView::UpdateStartAndEnd()
{
	const Camera2D& rayCamera = mCamera.GetRaylibCamera();
	const Vector2Int resolution(mCamera.GetResolution());
	const Vector2Int tileSize(mProject->GetTileSize());
	const Vector2Int first((int)rayCamera.target.x % tileSize.x, (int)rayCamera.target.y % tileSize.y);


	mStart = mCamera.TransformPoint(first - tileSize);
	mEnd = mCamera.TransformPoint(resolution + tileSize);
}

void SceneView::UpdateProject()
{	
	if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
		return;

	bool left = ImGui::IsMouseDown(ImGuiMouseButton_Left);
	bool right = ImGui::IsMouseDown(ImGuiMouseButton_Right);

	if ((left || right) && IsInsideTexture(::GetMousePosition()))
	{	
		Vector2Int gridPosition(ScreenToGrid(this->GetMousePosition(), true));
		if (left && !mEditorValues.selection.HasSelection())
			mProject->SetTile(gridPosition, mCurrentTileData);
		else if (right)
		{
			if (mEditorValues.selection.HasSelection())
			{
				mEditorValues.selection.DeleteTilesFromSelection(mProject);
				mEditorValues.selection.CancelSelection();
			}	
			else
			{
				mProject->RemoveTile(gridPosition);
			}
		}
			
	}
}

void SceneView::DrawSettingsMenuItem()
{
	if (ImGui::BeginMenu("Settings"))
	{
		if (ImGui::IsItemHovered())
			mImGuiValues.isHoveringMenu = true;

		ImGui::DragFloat("Camera Zoom Speed", &mSettings.zoomSpeed, 0.25f, 0.25f, 10.f);
		ImGui::DragFloat("Camera Move Speed", &mSettings.moveSpeed, 0.005f, 0.005f, 10.f);
		ImGui::DragInt("Grid Thickness", &mSettings.gridThickness, 1.f, 1.f, 6);
		{
			auto color = Convert(mSettings.gridColor);
			if (ImGui::ColorEdit4("Grid Color", &color.x))
				mSettings.gridColor = Convert(color);
		}
		ImGui::Checkbox("Draw Grid", &mSettings.showGrid);
		ImGui::EndMenu();


	}
}

void SceneView::DrawPlayerMenu()
{
	if (!ImGui::BeginMenu("Player"))
		return;

	if (ImGui::IsItemHovered())
		mImGuiValues.isHoveringMenu = true;

	Vector2Int spawnPosition = mProject->GetPlayerStartPosition();
	if (ImGuiUtils::SerializeInt2("Player Spawn Position", spawnPosition))
		mProject->SetPlayerSpawnPosition(spawnPosition);


	ImGui::EndMenu();
	
}

void SceneView::DrawDebugMenuItem()
{	
	if (ImGui::BeginMenu("Debug"))
	{
		if (ImGui::IsItemHovered())
			mImGuiValues.isHoveringMenu = true;

		ImGui::Text("Start: %i, %i", mStart.x, mStart.y);
		ImGui::Text("End: %i, %i", mEnd.x, mEnd.y);
		ImGui::Text("Is Menu Hovered: %s", mImGuiValues.isHoveringMenu ? "true" : "false");
		DrawDebugMenuItems();
		ImGui::EndMenu();
	}
	
}

void SceneView::DrawFileMenu()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::IsItemHovered())
			mImGuiValues.isHoveringMenu = true;

		if (ImGui::Button("Save"))
			mProject->Save();

		if (ImGui::Button("Save As"))
		{
			auto editor(App::GetLayer<Editor>());
			if (editor.has_value())
			{
				editor->get().OpenSaveAsModal();
			}
		}
		
		if (ImGui::Button("Quit"))
			App::Close();

		ImGui::EndMenu();
	}
}

void SceneView::DrawEditMenu()
{
	if (!ImGui::BeginMenu("Edit"))
		return;

	if (ImGui::IsItemHovered())
		mImGuiValues.isHoveringMenu = true;

	if (ImGui::Button("Clear"))
		mProject->Clear();

	if (ImGui::Button("Eye Dropper"))
		mEditorValues.eyeDropper.Activate();

	if (ImGui::Button("Play"))
		App::ChangeState(App::State::Gameplay);

	ImGui::EndMenu();

}

void SceneView::DrawMiniTile()
{
	const auto& texture = mTileSelector->GetCurrentTexture();
	const auto position(this->GetMousePosition());
	const auto tileSize(mProject->GetTileSize());
	const auto texturePosition = mCurrentTileData.imagePosition * tileSize;

	const Rectangle source(texturePosition.x, texturePosition.y, tileSize.x, tileSize.y);
	const Rectangle dest(position.x - 15.f, position.y - 15.f, 25.f, 25.f);
	const Vector2 origin = { 0.0f, 0.0f };

	DrawTexturePro(texture, source, dest, origin, 0.f, mCurrentTileData.tint);
}



Vector2Int SceneView::ScreenToGrid(Vector2Int screenPosition, bool isTexturePosition)
{
	if (!isTexturePosition)
		screenPosition = GetTexturePoint(screenPosition);

	const int thickness = mSettings.showGrid ? mSettings.gridThickness : 0;
	const auto tileSize = (mProject->GetTileSize() + thickness);

	if (screenPosition.x < 0)
		screenPosition.x -= tileSize.x;
	if (screenPosition.y < 0)
		screenPosition.y -= tileSize.y;

	return Vector2Divide(screenPosition, tileSize);
}

Vector2Int SceneView::GridToScreen(Vector2Int gridPosition)
{	
	return Vector2Int((gridPosition * mProject->GetTileSize()) + ((gridPosition + 1) * mSettings.gridThickness));
}
