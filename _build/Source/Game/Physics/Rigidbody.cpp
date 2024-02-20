#include "pch.h"
#include "Rigidbody.h"

#include "PhysicsWorld.h"

Rigidbody::Rigidbody(Sprite* sprite, size_t layer, bool useGravity)
	: mSprite(sprite)
	, mUseGravity(useGravity)
	, mGravityScale(1.f)
	, mIsActive(true)
	, mLayer(layer)
	, mVelocity({0, 0})
	, mUseDeceleration(true)
	, mIsGrounded(false)
	, mDecelerate(false)
{
	PhysicsWorld::Get().AddRigidbody(this);
}

Rigidbody::~Rigidbody()
{
	PhysicsWorld::Get().RemoveRigidbody(this);
}

void Rigidbody::ApplyForce(Vector2 force)
{
	mVelocity = Vector2Add(mVelocity, force);
}

void Rigidbody::ApplyForceX(float x)
{
	mVelocity.x += x;
	mDecelerate = false;
}

void Rigidbody::ApplyForceY(float y)
{
	mVelocity.y += y;
	mDecelerate = false;
}

void Rigidbody::SetLayer(size_t layer)
{
	auto& world = PhysicsWorld::Get();
	if (world.IsValidLayer(layer))
		world.ChangeLayer(this, layer);
}

void Rigidbody::AddLayerToLayerMask(size_t layer)
{
	if (!PhysicsWorld::Get().IsValidLayer(layer)) return;
	if (std::find(mLayerMask.begin(), mLayerMask.end(), layer) != mLayerMask.end()) return;

	mLayerMask.push_back(layer);
}

void Rigidbody::RemoveLayerFromLayerMask(size_t layer)
{
	if (!PhysicsWorld::Get().IsValidLayer(layer)) return;
	std::erase(mLayerMask, layer);
}
