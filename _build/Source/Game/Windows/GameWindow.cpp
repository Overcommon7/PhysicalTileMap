#include "pch.h"
#include "GameWindow.h"

#include "../Player/Player.h"
#include "../../Editor/Project/Project.h"

#include "App/App.h"

GameWindow::ITextureWindowConstructor(GameWindow)
	, player(nullptr)
{
	hasMenuBar = true;
}

void GameWindow::Start(Project* project)
{
	this->project = project;
	player = new Player();
}

void GameWindow::Stop()
{
	delete player;
	player = nullptr;
}

void GameWindow::Update()
{
	UpdateStartAndEnd();
}

void GameWindow::RaylibDraw()
{
	if (project == nullptr)
		return;

	auto GridToScreen = [this](Vector2Int gridPosition) {
		return this->GridToScreen(gridPosition);
		};

	project->Draw(ScreenToGrid(start), ScreenToGrid(end), GridToScreen);
}

void GameWindow::ImGuiDraw()
{
	if (ImGui::BeginMenuBar())
	{
		DrawFileMenu();
		DrawEditMenu();
		DrawDebugMenu();
			
		ImGui::EndMenuBar();
	}

	ITextureWindow::ImGuiDraw();
}

void GameWindow::DrawFileMenu()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::Button("Quit"))
			App::Close();

		ImGui::EndMenu();
	}	
}

void GameWindow::DrawEditMenu()
{
	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::Button("Edit Mode"))
		{
			App::ChangeState(App::State::Editing);
		}

		ImGui::EndMenu();
	}
}

void GameWindow::DrawDebugMenu()
{
	if (ImGui::BeginMenu("Debug"))
	{
		ImGui::EndMenu();
	}
}

Vector2Int GameWindow::ScreenToGrid(Vector2Int screenPosition) const
{
	const Vector2Int tileSize(project->GetTileSize());

	if (screenPosition.x < 0)
		screenPosition.x -= tileSize.x;
	if (screenPosition.y < 0)
		screenPosition.y -= tileSize.y;

	return Vector2Divide(screenPosition, tileSize);
}

Vector2Int GameWindow::GridToScreen(Vector2Int gridPosition) const
{
	if (project == nullptr)
		return Vector2Int();

	return Vector2Int(gridPosition * project->GetTileSize());
}

void GameWindow::UpdateStartAndEnd()
{
	const Camera2D& rayCamera = camera.GetRaylibCamera();
	const Vector2Int resolution(camera.GetResolution());
	const Vector2Int tileSize(project->GetTileSize());
	const Vector2Int first((int)rayCamera.target.x % tileSize.x, (int)rayCamera.target.y % tileSize.y);


	start = camera.TransformPoint(first - tileSize);
	end = camera.TransformPoint(resolution + tileSize);
}
