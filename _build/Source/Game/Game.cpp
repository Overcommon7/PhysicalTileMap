#include "pch.h"
#include "Game.h"

#include "Windows/GameWindow.h"

Game::Game()
{
	game = new GameWindow("Game", Vector2Int(1920, 1080));
	windows.emplace_back(game);
}

void Game::OnStart(Project* project)
{
	game->Start(project);
}

void Game::Stop()
{
	game->Stop();
}

