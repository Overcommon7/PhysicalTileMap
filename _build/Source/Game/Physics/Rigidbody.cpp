#include "pch.h"
#include "Rigidbody.h"

#include "PhysicsWorld.h"
#include "ImGuiUtilities/ImGuiUtils.h"

#include "Game/Sprites/Sprite.h"


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
	for (int i = 0; i <= (int)CollisionEdge::Left; ++i)
	{
		auto edge = (CollisionEdge)i;
		mCollidedTiles.insert({ edge, {} });
	}
		

	PhysicsWorld::AddRigidbody(this);
	assert(sprite != nullptr);
}

Rigidbody::~Rigidbody()
{
	PhysicsWorld::RemoveRigidbody(this);
}

Vector2 Rigidbody::Simulate(int steps) const
{
	return PhysicsWorld::Simulate(this, steps);
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

const vector<TileData>& Rigidbody::GetTileData(CollisionEdge edge) const
{
	return mCollidedTiles.at(edge);
}

void Rigidbody::ImGuiDraw()
{
	ImGuiUtils::SerializeBool("Is Active", mIsActive);
	ImGuiUtils::SerializeVector2("Velocity", mVelocity, 0.1f);
	ImGuiUtils::SerializeFloat("Graivty Scale", mGravityScale, 0.1f);
	ImGuiUtils::SerializeFloat("Deceleration Speed", mDecerationSpeed, 0.05f);
	if (ImGuiUtils::SerializeBool("Use Gravity", mUseGravity) && !mUseGravity)
		mVelocity.y = 0;

	ImGuiUtils::SerializeBool("Use Deceleration", mUseDeceleration);
	ImGuiUtils::SerializeBool("Collide With Tilemap", mCollideWithTilemap);
	ImGuiUtils::SerializeBool("Is Trigger", mIsTrigger);

	ImGuiUtils::DrawBool("Is Decelerating", mDecelerate);
	ImGuiUtils::DrawBool("Is Grounded", mIsGrounded);
	ImGui::Text("LayerID: %zu", mLayer);
}

CopyValues Rigidbody::Copy() const
{
	Rectangle collider(mSprite->Collider());
	Vector2 size(collider.width, collider.height);
	Sprite* simulateSprite = new Sprite(mSprite->GetType(), mSprite->GetPosition(), size);
	Rigidbody* simulateBody = new Rigidbody(simulateSprite, mLayer, mUseGravity, mIsTrigger);

	simulateBody->mVelocity = mVelocity;
	simulateBody->mLayerMask = mLayerMask;
	simulateBody->mCollideWithTilemap = mCollideWithTilemap;
	simulateBody->mDecelerate = mDecelerate;
	simulateBody->mGravityScale = mGravityScale;
	simulateBody->mUseGravity = mUseGravity;
	simulateBody->mIsActive = mIsActive;
	simulateBody->mIsGrounded = mIsGrounded;
	simulateBody->mDecelerate = mDecelerate;
	simulateBody->mUseDeceleration = mUseDeceleration;

	return { simulateBody, simulateSprite };
}

void CopyValues::Delete()
{
	delete sprite;
	sprite = nullptr;

	delete rigidbody;
	rigidbody = nullptr;
}
