#include "pch.h"
#include "App.h"

#include "Editor/Editor.h"
#include "Game/Game.h"

App::App()
{
	assert(!isCreated);
	isCreated = true;

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

	layers.emplace_back(new Editor())->Initialize();
	layers.emplace_back(new Game())->Initialize();
}

App::~App()
{
	for (const auto& layer : layers)
		layer->Shutdown();

	layers.clear();

	rlImGuiShutdown();
	CloseWindow();

	isCreated = false;
}

bool App::Run()
{
	while (!WindowShouldClose())
	{
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

	return !shouldRestart;
}
