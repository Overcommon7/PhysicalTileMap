#include "pch.h"
#include "GameWindow.h"

#include "../Player/Player.h"
#include "../../Editor/Project/Project.h"

#include "Game/Physics/PhysicsWorld.h"
#include "Game/Camera/CameraMovement.h"
#include "Game/Physics/PhysicsWorld.h"


#include "App/App.h"

GameWindow::ITextureWindowConstructor(GameWindow)
	, mPlayer(nullptr)
	, mProject(nullptr)
{
	mHasMenuBar = true;
}

void GameWindow::Start(Project* project)
{
	mProject = project;
	
	auto position(GridToScreen(project->GetPlayerStartPosition()));
	auto size(project->GetTileSize() * 0.95f);
	mPlayer = new Player(position, size);

	mCamera.GetRaylibCamera().target = position;
	
	mFixedUpdate = std::move([this](const float timeStep) { CameraMovement::FixedUpdate(mCamera, mCameraValues, *mPlayer, timeStep); });

	PhysicsWorld::OnFixedTimeStep() += mFixedUpdate;
}

void GameWindow::Stop()
{
	PhysicsWorld::OnFixedTimeStep() -= mFixedUpdate;

	delete mPlayer;
	mPlayer = nullptr;
}

void GameWindow::Update()
{	
	mPlayer->Update();
	UpdateStartAndEnd();
}

void GameWindow::RaylibDraw()
{
	if (mProject == nullptr)
		return;

	auto GridToScreen = [this](Vector2Int gridPosition) {
		return this->GridToScreen(gridPosition);
		};

	mProject->Draw(ScreenToGrid(mStart), ScreenToGrid(mEnd), GridToScreen);
	mPlayer->Draw();

	PhysicsWorld::DebugDraw();
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
	const Vector2Int tileSize(mProject->GetTileSize());

	if (screenPosition.x < 0)
		screenPosition.x -= tileSize.x;
	if (screenPosition.y < 0)
		screenPosition.y -= tileSize.y;

	return Vector2Divide(screenPosition, tileSize);
}

Vector2Int GameWindow::GridToScreen(Vector2Int gridPosition) const
{
	if (mProject == nullptr)
		return Vector2Int();

	return Vector2Int(gridPosition * mProject->GetTileSize());
}

void GameWindow::UpdateStartAndEnd()
{
	const Camera2D& rayCamera = mCamera.GetRaylibCamera();
	const Vector2Int resolution(mCamera.GetResolution());
	const Vector2Int tileSize(mProject->GetTileSize());
	const Vector2Int first((int)rayCamera.target.x % tileSize.x, (int)rayCamera.target.y % tileSize.y);


	mStart = mCamera.TransformPoint(first - tileSize);
	mEnd = mCamera.TransformPoint(resolution + tileSize);
}
