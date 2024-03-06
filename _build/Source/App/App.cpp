#include "pch.h"
#include "App.h"

#include "Editor/Editor.h"
#include "Game/Game.h"

#include "ImGuiUtilities/ImGuiUtils.h"

App::App()
	: mQueriedStateChange(std::nullopt)
	, mState(State::Editing)
	, mShouldClose(false)
{
	assert(!sIsCreated);
	sIsCreated = true;
	sInstance = this;

	{
		SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
		InitWindow(500, 500, "OverTiled");
		while (!IsWindowReady());

		int monitor = GetCurrentMonitor();
		Vector2Int screenSize(GetMonitorWidth(monitor) * 0.75f, GetMonitorHeight(monitor) * 0.75f);
		SetWindowSize(screenSize.x, screenSize.y);
		SetWindowPosition(screenSize.x * 0.125f, screenSize.y * 0.125f);
		SetTargetFPS(GetMonitorRefreshRate(monitor));
		SetExitKey(KEY_NULL);
	}

	{
		rlImGuiSetup(true);
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}

	mEditor = new Editor();
	mGame = new Game();

	mLayers.emplace_back(mEditor)->Initialize();
	mLayers.emplace_back(mGame)->Initialize();
}

App::~App()
{
	for (const auto& layer : mLayers)
		layer->Shutdown();

	mLayers.clear();

	rlImGuiShutdown();
	CloseWindow();

	sIsCreated = false;
	sInstance = nullptr;
}

void App::Run()
{
	while (!WindowShouldClose() && !mShouldClose)
	{
		if (mQueriedStateChange)
		{
			mState = *mQueriedStateChange;
			mQueriedStateChange = std::nullopt;
		}

		Time::Update();
		ImGuiUtils::NewFrame();
		ILayer* currentLayer = mLayers[(int)mState].get();

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
	sInstance->mQueriedStateChange = state;

	if (state == State::Gameplay)
	{
		sInstance->mGame->OnStart(sInstance->mEditor->mCurrentProject.get());
	}
	else if (state == State::Editing)
	{
		sInstance->mGame->Stop();
	}
}

void App::Close()
{
	sInstance->mShouldClose = true;
}
