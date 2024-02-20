#include "pch.h"
#include "Rigidbody.h"

#include "PhysicsWorld.h"

Rigidbody::Rigidbody(Sprite* sprite, size_t layer, bool useGravity, bool isTrigger)
	: mSprite(sprite)
	, mUseGravity(useGravity)
	, mGravityScale(1.f)
	, mDecerationSpeed(100.f)
	, mIsActive(true)
	, mLayer(layer)
	, mVelocity({0, 0})
	, mUseDeceleration(true)
	, mIsGrounded(false)
	, mDecelerate(false)
	, mCollideWithTilemap(true)
	, mIsTrigger(isTrigger)
{
	PhysicsWorld::AddRigidbody(this);
	assert(sprite != nullptr, "Rigidbody: rigidbody cannot have a null sprite");
}

Rigidbody::~Rigidbody()
{
	PhysicsWorld::RemoveRigidbody(this);
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
	PhysicsWorld::ChangePhysicsLayer(this, layer);
}

void Rigidbody::AddLayerToLayerMask(size_t layer)
{
	if (std::find(mLayerMask.begin(), mLayerMask.end(), layer) != mLayerMask.end()) 
		return;

	mLayerMask.push_back(layer);
}

void Rigidbody::RemoveLayerFromLayerMask(size_t layer)
{
	std::erase(mLayerMask, layer);
}

Sprite* Rigidbody::GetSprite()
{
	return mSprite;
}

const Sprite* Rigidbody::GetSprite() const
{
	return mSprite;
}
