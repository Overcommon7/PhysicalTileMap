#include "pch.h"
#include "PhysicsWorld.h"

#include "Rigidbody.h"
#include "Game/Sprites/Sprite.h"

PhysicsWorld::PhysicsWorld()
{
	for (const auto layer : PhyicsLayer::Layers)
		mBodies.insert({ layer , {} });
}

PhysicsWorld::~PhysicsWorld()
{
	for (auto& [layer, bodies] : mBodies)
	{
		for (auto& body : bodies)
			body = nullptr;
		bodies.clear();
	}
	mBodies.clear();
}

void PhysicsWorld::Initialize()
{
	sWorld = make_unique<PhysicsWorld>();
}

void PhysicsWorld::Terminate()
{
	sWorld.reset();
}

void PhysicsWorld::Update()
{
	sWorld->InternalUpdate();
}

void PhysicsWorld::DebugDraw()
{
	sWorld->InternalDebugDraw();
}

void PhysicsWorld::AddRigidbody(Rigidbody* rigidbody)
{
	auto& layer = sWorld->mBodies[rigidbody->mLayer];

	if (std::find_if(layer.begin(), layer.end(), [rigidbody](Rigidbody* body) {
		return body->GetSprite()->GetID() == rigidbody->GetSprite()->GetID();
		}) != layer.end())
	{
		return;
	}

	layer.push_back(rigidbody);
}

void PhysicsWorld::RemoveRigidbody(Rigidbody* rigidbody)
{
	if (!sWorld->mBodies.contains(rigidbody->mLayer))
		return;

	auto& layer = sWorld->mBodies.at(rigidbody->mLayer);
		
	auto iter = std::find_if(layer.begin(), layer.end(), [rigidbody](Rigidbody* body) {
		return body->GetSprite()->GetID() == rigidbody->GetSprite()->GetID();
		});

	if (iter == layer.end())
		return;

	layer.erase(iter);
}

void PhysicsWorld::ChangePhysicsLayer(Rigidbody* rigidbody, size_t layer)
{
	if (layer == rigidbody->mLayer)
		return;

	if (!sWorld->mBodies.contains(rigidbody->mLayer))
		return;

	if (!sWorld->mBodies.contains(layer))
		return;

	auto& physicsLayer = sWorld->mBodies.at(rigidbody->mLayer);
	auto& newPhysicsLayer = sWorld->mBodies.at(layer);

	auto iter = std::find_if(physicsLayer.begin(), physicsLayer.end(), [rigidbody](Rigidbody* body) {
		return body->GetSprite()->GetID() == rigidbody->GetSprite()->GetID();
		});

	if (iter == physicsLayer.end())
		return;

	physicsLayer.erase(iter);
	newPhysicsLayer.push_back(rigidbody);
}

void PhysicsWorld::InternalUpdate()
{
	mTimer += Time::DeltaTime();
	while (mTimer >= TIME_STEP)
	{
		DoPhysicsStep();
		mTimer -= TIME_STEP;
	}
}

void PhysicsWorld::InternalDebugDraw()
{
}

void PhysicsWorld::DoPhysicsStep()
{
}

void PhysicsWorld::ApplyForces()
{
}

void PhysicsWorld::ResolveCollisions()
{
}

void PhysicsWorld::ApplyGravity(Rigidbody* body)
{
}

void PhysicsWorld::ApplyDeceleration(Rigidbody* body)
{
}
