#include "pch.h"
#include "PhysicsWorld.h"

#include "Rigidbody.h"
#include "Game/Sprites/Sprite.h"

#include "CollisionSolver.h"

PhysicsWorld::PhysicsWorld(Project* project)
	: mProject(project)
{
	for (const auto layer : PhysicsLayer::Layers)
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

void PhysicsWorld::Initialize(Project* project,
	const std::function<Vector2Int(Vector2Int)>& screenToGrid,
	const std::function<Vector2Int(Vector2Int)>& gridToScreen)
{
	sWorld = make_unique<PhysicsWorld>(project);
	sWorld->mGridToScreen = gridToScreen;
	sWorld->mScreenToGrid = screenToGrid;
}

void PhysicsWorld::Terminate()
{
	sWorld->mProject = nullptr;
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
	for (const auto& [layer, bodies] : mBodies)
	{
		for (auto rigidbody : bodies)
		{
			DrawRectangleLinesEx(rigidbody->mSprite->Collider(), 2, RED);
		}
	}
}

void PhysicsWorld::DoPhysicsStep()
{
	ApplyForces();
	SolveCollisions();
}

void PhysicsWorld::ApplyForces()
{
	for (const auto& [layer, bodies] : mBodies)
	{
		for (auto rigidbody : bodies)
		{
			if (!rigidbody->mIsActive)
				continue;

			rigidbody->mCollisions.clear();

			if (rigidbody->mUseGravity)
				ApplyGravity(rigidbody);

			if (rigidbody->mUseDeceleration)
				ApplyDeceleration(rigidbody);

			rigidbody->mDecelerate = true;
			auto velocity(Vector2Scale(rigidbody->mVelocity, TIME_STEP));
			rigidbody->mSprite->Translate(velocity);
		}	
	}
}

void PhysicsWorld::SolveCollisions()
{
	for (const auto& [layer, bodies] : mBodies)
	{
		for (auto rigidbody : bodies)
		{
			if (!rigidbody->mIsActive)
				continue;

			rigidbody->mIsGrounded = false;

			if (rigidbody->mCollideWithTilemap)
				CollisionSolver::RigidbodyTilemapCollision(rigidbody, mProject, mScreenToGrid, mGridToScreen);

			for (auto layer : rigidbody->mLayerMask)
			{
				for (auto other : mBodies.at(layer))
				{
					CollisionSolver::RigidbodyRigidbodyCollision(rigidbody, other);
				}
			}		
		}
	} 
}

void PhysicsWorld::ApplyGravity(Rigidbody* body)
{
	if (body->mGravityScale != 0)
	{
		float gravity = GRAVITY * body->mGravityScale;
		body->mVelocity.y += gravity * TIME_STEP;   
	}
}

void PhysicsWorld::ApplyDeceleration(Rigidbody* body)
{
	if (!body->mDecelerate)
		return;

	if (body->mVelocity.x < 0)
	{
		body->mVelocity.x += TIME_STEP * body->mDecerationSpeed;
		if (body->mVelocity.x > std::numeric_limits<float>::epsilon() * -2.f)
			body->mVelocity.x = 0;
	}
	else if (body->mVelocity.x > 0)
	{
		body->mVelocity.x -= TIME_STEP * body->mDecerationSpeed;
		if (body->mVelocity.x < std::numeric_limits<float>::epsilon() * 2.f)
			body->mVelocity.x = 0;
	} 
}
