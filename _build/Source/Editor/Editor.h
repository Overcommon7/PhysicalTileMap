#pragma once
#include "App/ILayer.h"
#include "Project/Project.h"

class SceneView;
class TileSelector;

class Editor : public ILayer
{
public:
	Editor();
	~Editor();
private:
	
	SceneView* sceneView;
	TileSelector* tileSelector;

	unique_ptr<Project> currentProject;
};

