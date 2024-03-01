#pragma once
#include "App/ILayer.h"
#include "Project/Project.h"

class SceneView;
class TileSelector;

class Editor final : public ILayer
{
public:
	Editor();
	~Editor();

	const TileSelector* const GetTileSelector() const { return tileSelector; }
	void Update() override;
private:
	
	SceneView* sceneView;
	TileSelector* tileSelector;

	unique_ptr<Project> currentProject;

	friend class App;
};

