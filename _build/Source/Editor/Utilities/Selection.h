#pragma once

class Project;
class SceneView;
class TileSelector;
struct TileData;

class Selection
{	
public:
	void BeginDragPoint(Vector2Int gridPosition);
	void UpdateEndPoint(Vector2Int endPoint);

	void Update(SceneView* const sceneView, const TileSelector* const tileSelector);
	void RaylibDraw(SceneView* const sceneView);

	void DeleteTilesFromSelection(Project* project);
	void FillSelectedTiles(Project* project, const TileData& tileData);

	void CancelSelection();
	void OnNewDataSelected(Project* project, const TileData& data);

	bool HasSelection() const { return hasSelection; }
	Vector2Int GetSelectionStart() const { return selectionStart; }
	Vector2Int GetSelectionEnd() const { return selectionEnd; }
	Rectangle GetSelectionScreenRec(const std::function<Vector2Int(Vector2Int)> gridToScreen) const;

private:
	Vector2Int selectionStart;
	Vector2Int selectionEnd;

	bool hasSelection = false;

	
};

