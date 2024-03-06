#include "pch.h"
#include "Editor.h"

#include "Windows/SceneView.h"
#include "Windows/TileSelector.h"

#include "App/App.h"

Editor::Editor()
{
	mCurrentProject = std::make_unique<Project>("Projects/Test/Test.txt");

	mSceneView = new SceneView("Scene View", { 1280, 720 });
	mTileSelector = new TileSelector("Tile Selector", { 360, 720 });

	mSceneView->SetProject(mCurrentProject.get());
	mTileSelector->SetProject(mCurrentProject.get());

	mWindows.emplace_back(mSceneView);	
	mWindows.emplace_back(mTileSelector);	

	auto OnDataChanged = [this](TileData& tileData) {
		mSceneView->SetNewTileData(tileData);
		};

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

	if (!mSaveAsModal) 
		return;

	mSaveAsModal->ImGuiDrawBegin();
	mSaveAsModal->ImGuiDraw();
	mSaveAsModal->ImGuiDrawEnd();

	if (!mSaveAsModal->IsClosed())
		return;

	if (mSaveAsModal->ShouldSave())
	{
		mCurrentProject->SaveAs(mSaveAsModal->GetSavePath());
	}

	mSaveAsModal.reset();
}

void Editor::OpenSaveAsModal()
{
	if (!mSaveAsModal)
		mSaveAsModal = std::make_unique<SaveAsModal>();
}
