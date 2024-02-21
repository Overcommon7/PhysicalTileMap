#include "pch.h"
#include "Game.h"
#include "Game/Physics/PhysicsWorld.h"

#include "Windows/GameWindow.h"


Game::Game()
{
	game = new GameWindow("Game", Vector2Int(1920, 1080));
	windows.emplace_back(game);
}

void Game::OnStart(Project* project)
{
	auto GridToScreen = [this](Vector2Int position) {
		return game->GridToScreen(position);
		};

	auto ScreenToGrid = [this](Vector2Int position) {
		return game->ScreenToGrid(position);
		};

	PhysicsWorld::Initialize(project, ScreenToGrid, GridToScreen);

	game->Start(project);
}

void Game::Stop()
{
	game->Stop();

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
	PhysicsWorld::DebugDraw();
}

