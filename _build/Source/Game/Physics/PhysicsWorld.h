#pragma once
#include "PhyicsLayers.h"
class Rigidbody;

class PhysicsWorld final
{
	PhysicsWorld();
	~PhysicsWorld();

	static void Initialize();
	static void Terminate();

	static void Update();
	static void DebugDraw();

	static void AddRigidbody(Rigidbody* rigidbody);
	static void RemoveRigidbody(Rigidbody* rigidbody);
	static void ChangePhysicsLayer(Rigidbody* rigidbody, size_t layer);
private:

	float mTimer = 0.f;
	unordered_map<size_t, vector<Rigidbody*>> mBodies;

	void InternalUpdate();
	void InternalDebugDraw();
	void DoPhysicsStep();
	void ApplyForces();
	void ResolveCollisions();
	void ApplyGravity(Rigidbody* body);
	void ApplyDeceleration(Rigidbody* body);

	static inline std::hash<string> sHasher = {};
	static inline unique_ptr<PhysicsWorld> sWorld = nullptr;
	static inline constexpr float GRAVITY = 550.f;
	static inline constexpr float TIME_STEP = 1 / 60.f;
};

