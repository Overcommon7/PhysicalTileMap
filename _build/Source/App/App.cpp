#include "pch.h"
#include "App.h"

#include "Editor/Editor.h"
#include "Game/Game.h"

App::App()
	: queriedStateChange(std::nullopt)
	, state(State::Editing)
	, shouldClose(false)
{
	assert(!isCreated);
	isCreated = true;
	instance = this;

	{
		constexpr int screenWidth = 1280;
		constexpr int screenHeight = 720;

		SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
		InitWindow(screenWidth, screenHeight, "OverTiled");
		SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
		SetExitKey(KEY_NULL);
	}

	{
		rlImGuiSetup(true);
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}

	editor = new Editor();
	game = new Game();

	layers.emplace_back(editor)->Initialize();
	layers.emplace_back(game)->Initialize();
}

App::~App()
{
	for (const auto& layer : layers)
		layer->Shutdown();

	layers.clear();

	rlImGuiShutdown();
	CloseWindow();

	isCreated = false;
	instance = nullptr;
}

void App::Run()
{
	while (!WindowShouldClose() && !shouldClose)
	{
		if (queriedStateChange)
		{
			state = *queriedStateChange;
			queriedStateChange = std::nullopt;
		}

		Time::Update();
		ILayer* currentLayer = layers[(int)state].get();

		currentLayer->Update();

		BeginDrawing();
		{
			ClearBackground(BLACK);
			currentLayer->RaylibDraw();

			rlImGuiBegin();
			{
				ImGui::DockSpaceOverViewport();
				currentLayer->ImGuiDraw();			
			}
			rlImGuiEnd();
		}
		EndDrawing();
	}
}

void App::ChangeState(State state)
{
	instance->queriedStateChange = state;

	if (state == State::Gameplay)
	{
		instance->game->OnStart(instance->editor->currentProject.get());
	}
	else if (state == State::Editing)
	{
		instance->game->Stop();
	}
}

void App::Close()
{
	instance->shouldClose = true;
}
