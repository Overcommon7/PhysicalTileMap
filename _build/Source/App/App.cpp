#include "pch.h"
#include "App.h"

#include "Editor/Editor.h"

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

	editor = new Editor();
}

App::~App()
{
	delete editor;

	rlImGuiShutdown();
	CloseWindow();

	isCreated = false;
}

bool App::Run()
{
	while (!WindowShouldClose())
	{
		Time::Update();

		editor->Update();

		BeginDrawing();
		{
			ClearBackground(BLACK);
			editor->RaylibDraw();

			rlImGuiBegin();
			{
				ImGui::DockSpaceOverViewport();
				editor->ImGuiDraw();			
			}
			rlImGuiEnd();
		}
		EndDrawing();		
	}

	return !shouldRestart;
}
