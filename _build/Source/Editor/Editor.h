#pragma once
#include "App/ILayer.h"
#include "Project/Project.h"
#include "Windows/SaveAsModal.h"
#include "Windows/OpenProjectModal.h"

class SceneView;
class TileSelector;


class Editor final : public ILayer
{
public:
	Editor();
	~Editor();

	const TileSelector* const GetTileSelector() const { return mTileSelector; }
	void Update() override;
	void ImGuiDraw() override;
	void OpenSaveAsModal();

private:
	
	SceneView* mSceneView;
	TileSelector* mTileSelector;
	SaveAsModal* mSaveAsModal;
	OpenProjectModal* mOpenProjectModal;
	
	unique_ptr<Project> mCurrentProject;

	friend class App;
};

