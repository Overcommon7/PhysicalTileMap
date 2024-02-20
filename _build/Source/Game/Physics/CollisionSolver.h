#pragma once
class Rigidbody;
class Project;

struct RectangleCorners
{
	struct Position
	{ 
		Vector2Int left;
		Vector2Int right;
	};

	Position top;
	Position bottom;
};

class CollisionSolver
{
public:
	using Conversion = const std::function<Vector2Int(Vector2Int)>&;

	static void RigidbodyRigidbodyCollision(Rigidbody* rigidbody, Rigidbody* other);
	static void RigidbodyTilemapCollision(Rigidbody* rigidbody, Project* project, Conversion screenToGrid, Conversion gridToScreen);

private:
	static bool TryGetRectangleOverlap(Rectangle rec1, Rectangle rec2, Rectangle& overlap);
	static RectangleCorners GetRectangleCorners(Rectangle collider, Conversion screenToGrid);

	static Vector2Int SolveTopRightCorner(Rigidbody* rigidbody, Vector2Int corner, Project* project);
	static Vector2Int SolveTopLeftCorner(Rigidbody* rigidbody, Vector2Int corner, Project* project);
	static Vector2Int SolveBottomRightCorner(Rigidbody* rigidbody,Vector2Int corner, Project* project);
	static Vector2Int SolveBottomLeftCorner(Rigidbody* rigidbody, Vector2Int corner, Project* project);
};

