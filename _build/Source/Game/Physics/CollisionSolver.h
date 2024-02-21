#pragma once
class Rigidbody;
class Project;

class CollisionSolver
{
public:
	using Conversion = const std::function<Vector2Int(Vector2Int)>&;

	static void RigidbodyRigidbodyCollision(Rigidbody* rigidbody, Rigidbody* other);
	static void RigidbodyTilemapCollision(Rigidbody* rigidbody, Project* project, Conversion screenToGrid, Conversion gridToScreen);

private:
	static bool TryGetRectangleOverlap(Rectangle rec1, Rectangle rec2, Rectangle& overlap);

	static void SolveTopRightCorner(Rigidbody* rigidbody, Vector2Int corner, Project* project, Conversion gridToScreen, bool& stopX, bool& stopY);
	static void SolveTopLeftCorner(Rigidbody* rigidbody, Vector2Int corner, Project* project, Conversion gridToScreen, bool& stopX, bool& stopY);
	static void SolveBottomRightCorner(Rigidbody* rigidbody,Vector2Int corner, Project* project, Conversion gridToScreen, bool& stopX, bool& stopY);
	static void SolveBottomLeftCorner(Rigidbody* rigidbody, Vector2Int corner, Project* project, Conversion gridToScreen, bool& stopX, bool& stopY);
};

