#pragma once
class Rigidbody;
class Project;

class CollisionSolver
{
public:
	using Conversion = const std::function<Vector2Int(Vector2Int)>&;
	

	static void RigidbodyRigidbodyCollision(Rigidbody* rigidbody, Rigidbody* other);
	static void RigidbodyTilemapCollision(Rigidbody* rigidbody, Project* project, Conversion screenToGrid, Conversion gridToScreen);

	static void ImGuiDraw();
private:
	inline static size_t sCollisionsSolved = 0;
	inline static int sCollisionsSolvedThisSecond = 0;
	inline static int sCollisionsSolvedLastSecond = 0;
	inline static float timer = 0;

	static bool TryGetRectangleOverlap(Rectangle rec1, Rectangle rec2, Rectangle& overlap);

	static void SolveTopRightCorner(Rigidbody* rigidbody, Vector2Int corner, Vector2Int offset, Conversion gridToScreen, bool& stopX, bool& stopY);
	static void SolveTopLeftCorner(Rigidbody* rigidbody, Vector2Int corner, Vector2Int offset, Conversion gridToScreen, bool& stopX, bool& stopY);
	static void SolveBottomRightCorner(Rigidbody* rigidbody,Vector2Int corner, Vector2Int offset, Conversion gridToScreen, bool& stopX, bool& stopY);
	static void SolveBottomLeftCorner(Rigidbody* rigidbody, Vector2Int corner, Vector2Int offset, Conversion gridToScreen, bool& stopX, bool& stopY);

	static void SolveCorner(
		const std::function<void(Rigidbody*, Vector2Int, Vector2Int, Conversion, bool&, bool&)> solver,
		Rigidbody* rigidbody, Project* project, Vector2Int corner, Vector2Int offset, 
		Conversion gridToScreen, bool& stopX, bool& stopY);
		
};

