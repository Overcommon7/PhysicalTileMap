#include "pch.h"
#include "Editor.h"

#include "Windows/SceneView.h"
#include "Windows/TileSelector.h"

Editor::Editor()
{
	currentProject = std::make_unique<Project>("_build/Projects/Test/Test.txt");

	sceneView = new SceneView("Scene View", { 1280, 720 });
	tileSelector = new TileSelector("Tile Selector", { 360, 720 });

	sceneView->SetProject(currentProject.get());
	tileSelector->SetProject(currentProject.get());

	windows.emplace_back(sceneView);	
	windows.emplace_back(tileSelector);	

	auto OnDataChanged = [this](TileData& tileData) {
		sceneView->SetNewTileData(tileData);
		};

	tileSelector->OnDataChanged += OnDataChanged;
	sceneView->SetTileSelector(tileSelector);
	sceneView->SetNewTileData(tileSelector->GetSelectedTileData());
}

Editor::~Editor()
{
	FileData::Terminate();
}
