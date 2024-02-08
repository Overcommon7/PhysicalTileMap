#include "pch.h"
#include "SceneView.h"

#include "../Project/Project.h"

void SceneView::SetProject(Project* project)
{
	this->project = project;
	SetWindowTitle(project->GetWindowTitle().c_str());
}

void SceneView::SetNewTileData(const TileData& data)
{
	currentTileData = data;
}

void SceneView::RaylibDraw()
{
	if (project == nullptr)
		return;

	auto GridToScreen = [this](Vector2Int grid) -> Vector2Int
		{
			return this->GridToScreen(grid);
		};

	if (settings.showGrid)
		DrawGrid();

	project->Draw(ScreenToGrid(start, true), ScreenToGrid(end, true), GridToScreen);
}

void SceneView::ImGuiDraw()
{
	if (ImGui::BeginMenuBar())
	{
		imGuiValues.isHoveringMenu = false;
		DrawFileMenu();
		DrawSettingsMenuItem();
		DrawDebugMenuItem();
		ImGui::EndMenuBar();
	}

	if (project == nullptr)
		return;

	UpdateCamera();

	if (!imGuiValues.isHoveringMenu)
		UpdateProject();
	
	ITextureWindow::ImGuiDraw();
}

void SceneView::ImGuiDrawBegin()
{
	ImGui::Begin(GetImGuiTitle(),  isClosable ? &isClosed : nullptr, ImGuiWindowFlags_MenuBar);
}

void SceneView::Update()
{
	if (project == nullptr)
		return;

	UpdateStartAndEnd();
}

void SceneView::UpdateCamera()
{
	Camera2D& rayCamera = camera.GetRaylibCamera();

	if (ImGui::IsWindowHovered())
	{
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
		{
			Vector2Int vec(ImGui::GetMouseDragDelta(ImGuiMouseButton_Middle));
			ImGui::ResetMouseDragDelta(ImGuiMouseButton_Middle);
			rayCamera.target = Vector2Add(rayCamera.target, (Vector2Scale(vec, settings.moveSpeed * (-1 / rayCamera.zoom))));
		}

		auto wheel = GetMouseWheelMoveV().y;

		if (wheel != 0)
		{
			rayCamera.zoom += (wheel * Time::DeltaTime() * settings.zoomSpeed);
			rayCamera.zoom = std::clamp(rayCamera.zoom, 0.5f, 5.f);
		}
	}
}

void SceneView::DrawGrid()
{
	const Camera2D& rayCamera = camera.GetRaylibCamera();
	const int thickness = settings.showGrid ? settings.gridThickness : 0;

	const Vector2Int tileSize(project->GetTileSize() + thickness);	
	const Vector2Int dimensions(end - start + tileSize);

	for (int x = start.x - (start.x % tileSize.x); x < end.x; x += tileSize.x)
	{
		DrawRectangle(x, start.y, thickness, dimensions.y, settings.gridColor);
	}

	for (int y = start.y - (start.y % tileSize.y); y < end.y; y += tileSize.y)
	{
		DrawRectangle(start.x, y, dimensions.x, thickness, settings.gridColor);
	}
}

void SceneView::UpdateStartAndEnd()
{
	const Camera2D& rayCamera = camera.GetRaylibCamera();
	const Vector2Int resolution(camera.GetResolution());
	const Vector2Int tileSize(project->GetTileSize());
	const Vector2Int first((int)rayCamera.target.x % tileSize.x, (int)rayCamera.target.y % tileSize.y);


	start = camera.TransformPoint(first - tileSize);
	end = camera.TransformPoint(resolution + tileSize);
}

void SceneView::UpdateProject()
{	
	if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && IsInsideTexture(::GetMousePosition()))
	{	
		Vector2Int gridPosition(ScreenToGrid(this->GetMousePosition(), true));
		project->SetTile(gridPosition, currentTileData);
	}
}

void SceneView::DrawSettingsMenuItem()
{
	if (ImGui::BeginMenu("Settings"))
	{
		if (ImGui::IsItemHovered())
			imGuiValues.isHoveringMenu = true;

		ImGui::DragFloat("Camera Zoom Speed", &settings.zoomSpeed, 0.25f, 0.25f, 10.f);
		ImGui::DragFloat("Camera Move Speed", &settings.moveSpeed, 0.005f, 0.005f, 10.f);
		ImGui::DragInt("Grid Thickness", &settings.gridThickness, 1.f, 1.f, 6);
		{
			auto color = Convert(settings.gridColor);
			if (ImGui::ColorEdit4("Grid Color", &color.x))
				settings.gridColor = Convert(color);
		}
		ImGui::Checkbox("Draw Grid", &settings.showGrid);
		ImGui::EndMenu();


	}
}

void SceneView::DrawDebugMenuItem()
{	
	if (ImGui::BeginMenu("Debug"))
	{
		if (ImGui::IsItemHovered())
			imGuiValues.isHoveringMenu = true;

		Vector2Int gridPosition(ScreenToGrid(this->GetMousePosition(), true));

		ImGui::Text("Start: %i, %i", start.x, start.y);
		ImGui::Text("End: %i, %i", end.x, end.y);
		ImGui::Text("Mouse Grid Position: %i, %i", gridPosition.x, gridPosition.y);
		ImGui::Text("Is Menu Hovered: %s", imGuiValues.isHoveringMenu ? "true" : "false");
		DrawDebugMenuItems();
		ImGui::EndMenu();
	}
	
}

void SceneView::DrawFileMenu()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::IsItemHovered())
			imGuiValues.isHoveringMenu = true;

		if (ImGui::Button("Save"))
		{
			project->Save();
		}

		ImGui::EndMenu();
	}
}

Vector2Int SceneView::ScreenToGrid(Vector2Int screenPosition, bool isTexturePosition)
{
	if (!isTexturePosition)
		screenPosition = GetTexturePoint(screenPosition);

	const int thickness = settings.showGrid ? settings.gridThickness : 0;
	const auto tileSize = (project->GetTileSize() + thickness);

	if (screenPosition.x < 0)
		screenPosition.x -= tileSize.x;
	if (screenPosition.y < 0)
		screenPosition.y -= tileSize.y;

	return Vector2Divide(screenPosition, tileSize);
}

Vector2Int SceneView::GridToScreen(Vector2Int gridPosition)
{	
	return Vector2Int((gridPosition * project->GetTileSize()) + ((gridPosition + 1) * settings.gridThickness));
}