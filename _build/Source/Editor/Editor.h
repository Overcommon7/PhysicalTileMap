#pragma once
#include "ImGuiWindow/IWindow.h"
#include "Project/Project.h"

class SceneView;
class TileSelector;

class Editor
{
public:
	Editor();
	~Editor();

	void Update();
	void RaylibDraw();
	void ImGuiDraw();

	vector<unique_ptr<IWindow>> windows;
	unique_ptr<Project> currentProject;
private:
	
	SceneView* sceneView;
	TileSelector* tileSelector;
};

