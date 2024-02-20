#pragma once
#include "PhysicsLayers.h"
class Rigidbody;
class Project;

class PhysicsWorld final
{
public:
	PhysicsWorld(Project* project);
	~PhysicsWorld();

	static void Initialize(Project* project,
		const std::function<Vector2Int(Vector2Int)>& screenToGrid,
		const std::function<Vector2Int(Vector2Int)>& gridToScreen);

	static void Terminate();
	static void Update();
	static void DebugDraw();

	static void AddRigidbody(Rigidbody* rigidbody);
	static void RemoveRigidbody(Rigidbody* rigidbody);
	static void ChangePhysicsLayer(Rigidbody* rigidbody, size_t layer);
private:

	float mTimer = 0.f;
	Project* mProject;
	unordered_map<size_t, vector<Rigidbody*>> mBodies;

	std::function<Vector2Int(Vector2Int)> mScreenToGrid = {};
	std::function<Vector2Int(Vector2Int)> mGridToScreen = {};

	void InternalUpdate();
	void InternalDebugDraw();
	void DoPhysicsStep();
	void ApplyForces();
	void SolveCollisions();
	void ApplyGravity(Rigidbody* body);
	void ApplyDeceleration(Rigidbody* body);

	
	static inline std::hash<string> sHasher = {};
	static inline unique_ptr<PhysicsWorld> sWorld = nullptr;
	static inline constexpr float GRAVITY = 550.f;
	static inline constexpr float TIME_STEP = 1 / 60.f;
};

