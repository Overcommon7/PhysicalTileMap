#include "pch.h"
#include "Game.h"
#include "Game/Physics/PhysicsWorld.h"

#include "Windows/GameWindow.h"
#include "Windows/DebugWindow.h"


Game::Game()
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

	mGame->Start(project);
	mDebug->SetPlayer(mGame->GetPlayer());
}

void Game::Stop()
{
	mDebug->SetPlayer(nullptr);
	mGame->Stop();

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
}

