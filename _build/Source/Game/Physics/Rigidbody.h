#pragma once
#include "PhysicsLayers.h"
class Sprite;

struct Collision
{
	Rectangle overlap;
	Sprite* sprite;

	Collision(Sprite* sprite, Rectangle overlap)
		: sprite(sprite), overlap(overlap) {}
};

class Rigidbody
{
public:
	Rigidbody(Sprite* sprite, size_t layer, bool useGravity = true);
	~Rigidbody();

	void ApplyForce(Vector2 force);
	void ApplyForceX(float x);
	void ApplyForceY(float y);

	void SetVelocity(Vector2 velocity) { mVelocity = velocity; }
	void SetVelocityX(float xVelocity) { mVelocity.x = xVelocity; }
	void SetVelocityY(float yVelocity) { mVelocity.y = yVelocity; }
	void SetDecelerationScale(float scale) { mDecerationScale = scale; }
	void SetGravityScale(float scale) { mGravityScale = scale; }
	void SetActive(bool active) { mIsActive = active; }
	void SetLayer(size_t layer);
	void UseDeceleration(bool decelrate) { mUseDeceleration = decelrate; }

	void AddLayerToLayerMask(size_t layer);
	void RemoveLayerFromLayerMask(size_t layer);
	const vector<size_t>& GetLayerMask() const { return mLayerMask; }


	Vector2 GetVelocity() const { return mVelocity; }
	float GetGravityScale() const { return mGravityScale; }
	float GetDecelrationScale() const { return mDecerationScale; }
	size_t GetLayer() const { return mLayer; }
	Sprite* GetSprite();
	const Sprite* GetSprite() const;
	bool IsUsingGravity() const { return mUseGravity; }
	bool IsActive() const { return mIsActive; }
	bool IsGrounded() const { return mIsGrounded; }

	const vector<Collision>& GetCollisions() const { return mCollisions; }
private:
	Vector2 mVelocity;
	float mGravityScale;
	float mDecerationScale;
	bool mUseGravity;
	bool mIsActive;
	bool mIsGrounded;
	bool mDecelerate;
	bool mUseDeceleration;
	size_t mLayer;
	Sprite* mSprite;

	vector<size_t> mLayerMask;
	vector<Collision> mCollisions;


	friend class PhysicsWorld;
};

