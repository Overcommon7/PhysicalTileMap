#include "pch.h"
#include "Editor.h"

#include "Windows/SceneView.h"
#include "Windows/TileSelector.h"

#include "App/App.h"

Editor::Editor()
{
	mCurrentProject = nullptr;

	mSceneView = new SceneView("Scene View", { 1280, 720 });
	mTileSelector = new TileSelector("Tile Selector", { 360, 720 });
	mSaveAsModal = new SaveAsModal();
	mOpenProjectModal = new OpenProjectModal();
	
	mWindows.emplace_back(mSceneView);	
	mWindows.emplace_back(mTileSelector);
	mWindows.emplace_back(mSaveAsModal);
	mWindows.emplace_back(mOpenProjectModal);

	auto OnDataChanged = [this](TileData& tileData) {
		mSceneView->SetNewTileData(tileData);
		};

	auto OnProjectOpened = [this](fs::path& path) {
		mCurrentProject = std::make_unique<Project>(path);
		mSceneView->SetProject(mCurrentProject.get());
		mTileSelector->SetProject(mCurrentProject.get());
		};

	mOpenProjectModal->OnProjectOpened += OnProjectOpened;
	mTileSelector->OnDataChanged += OnDataChanged;
	mSceneView->SetTileSelector(mTileSelector);
	mSceneView->SetNewTileData(mTileSelector->GetSelectedTileData());
}

Editor::~Editor()
{
	FileData::Terminate();
}

void Editor::Update()
{
	ILayer::Update();

	if (!IsKeyDown(KEY_LEFT_CONTROL) && !IsKeyDown(KEY_RIGHT_CONTROL))
		return;

	if (IsKeyPressed(KEY_P))
	{
		App::ChangeState(App::State::Gameplay);
		return;
	}
	
	if (mCurrentProject && IsKeyPressed(KEY_S))
	{
		if (IsKeyDown(KEY_RIGHT_SHIFT) || IsKeyDown(KEY_LEFT_SHIFT))
		{
			OpenSaveAsModal();
		}
		else
		{
			mCurrentProject->Save();
		}
	}
}

void Editor::ImGuiDraw()
{
	ILayer::ImGuiDraw();

	if (mSaveAsModal->ShouldSave())
	{		
		auto savePath(mSaveAsModal->GetSavePath() / (mCurrentProject->GetSavePath().stem().string() + ".txt"));
		mCurrentProject->SaveAs(savePath);
		mSaveAsModal->Close();
		mSaveAsModal->Saved();
	}
}

void Editor::OpenSaveAsModal()
{
	mSaveAsModal->Open();
}
