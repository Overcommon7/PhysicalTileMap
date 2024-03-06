#include "pch.h"
#include "Game.h"
#include "Game/Physics/PhysicsWorld.h"

#include "Windows/GameWindow.h"
#include "Windows/DebugWindow.h"

#include "App/App.h"


Game::Game()
	: mProject(nullptr)
{
	mGame = new GameWindow("Game", Vector2Int(1920, 1080));
	mDebug = new DebugWindow("Debug");

	mWindows.emplace_back(mGame);
	mWindows.emplace_back(mDebug);
}

void Game::OnStart(Project* project)
{
	auto GridToScreen = [this](Vector2Int position) {
		return mGame->GridToScreen(position);
		};

	auto ScreenToGrid = [this](Vector2Int position) {
		return mGame->ScreenToGrid(position);
		};

	PhysicsWorld::Initialize(project, ScreenToGrid, GridToScreen);

	mProject = project;
	mGame->Start(project);
	mDebug->SetPlayer(mGame->GetPlayer());
}

void Game::Stop()
{
	mDebug->SetPlayer(nullptr);
	mGame->Stop();
	mProject = nullptr;

	PhysicsWorld::Terminate();
}

void Game::Update()
{
	ILayer::Update();
	PhysicsWorld::Update();
}

void Game::RaylibDraw()
{
	ILayer::RaylibDraw();
	HandleHotkeys();
}

void Game::HandleHotkeys()
{
	if (IsKeyPressed(KEY_ESCAPE))
	{
		App::ChangeState(App::State::Editing);
		return;
	}

	if (!IsKeyDown(KEY_LEFT_CONTROL) && !IsKeyDown(KEY_RIGHT_CONTROL))
		return;

	if (IsKeyPressed(KEY_R))
	{
		Project* project = mProject;
		Stop();
		OnStart(project);
	}

}

