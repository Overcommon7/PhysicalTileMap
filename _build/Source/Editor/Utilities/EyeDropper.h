#pragma once
class Project;
class TileSelector;

class EyeDropper
{
public:
	void Select(Project* project, TileSelector* tileSelector, Vector2Int mouseGridPosition);
	void Update();

	void Activate();
	void Cancel();

	bool IsActive();


private:
	bool mIsActive = false;
};

