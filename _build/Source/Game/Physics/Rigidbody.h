#pragma once
class Sprite;
class Rigidbody;

struct Collision
{
	Rectangle overlap;
	Rigidbody* rigidbody;

	Collision(Rigidbody* rigidbody, Rectangle overlap)
		: rigidbody(rigidbody), overlap(overlap) 
	{
		assert(rigidbody != nullptr);
		assert(overlap.width != -1 && overlap.height != -1);
	}
};

struct CopyValues
{
	Rigidbody* rigidbody;
	Sprite* sprite;

	void Delete();
};

enum class CollisionEdge
{
	Top,
	Bottom,
	Right,
	Left
};

class Rigidbody
{
public:
	Rigidbody(Sprite* sprite, size_t layer, bool useGravity = true, bool isTrigger = false);
	~Rigidbody();

	Vector2 Simulate(int steps) const;

	void ApplyForce(Vector2 force);
	void ApplyForceX(float x);
	void ApplyForceY(float y);

	void SetVelocity(Vector2 velocity) { mVelocity = velocity; }
	void SetVelocityX(float xVelocity) { mVelocity.x = xVelocity; }
	void SetVelocityY(float yVelocity) { mVelocity.y = yVelocity; }
	void SetDecelerationSpeed(float speed) { mDecerationSpeed = speed; }
	void SetGravityScale(float scale) { mGravityScale = scale; }
	void SetActive(bool active) { mIsActive = active; }
	void SetLayer(size_t layer);
	void UseDeceleration(bool decelrate) { mUseDeceleration = decelrate; }

	void AddLayerToLayerMask(size_t layer);
	void RemoveLayerFromLayerMask(size_t layer);
	const vector<size_t>& GetLayerMask() const { return mLayerMask; }

	Vector2 GetVelocity() const { return mVelocity; }
	float GetGravityScale() const { return mGravityScale; }
	size_t GetLayer() const { return mLayer; }
	Sprite* GetSprite();
	const Sprite* GetSprite() const;
	bool IsUsingGravity() const { return mUseGravity; }
	bool IsActive() const { return mIsActive; }
	bool IsGrounded() const { return mIsGrounded; }

	void CollidesWithTilemap(bool collides) { mCollideWithTilemap = collides; }
	bool CollidesWithTilemap() { return mCollideWithTilemap; }

	const vector<Collision>& GetCollisions() const { return mCollisions; }

	void ImGuiDraw();
private:
	Vector2 mVelocity;
	float mGravityScale;
	float mDecerationSpeed;
	bool mUseGravity;
	bool mIsActive;
	bool mIsGrounded;
	bool mDecelerate;
	bool mUseDeceleration;
	bool mCollideWithTilemap;
	bool mIsTrigger;
	size_t mLayer;
	Sprite* mSprite;

	vector<size_t> mLayerMask;
	vector<Collision> mCollisions;


	CopyValues Copy() const;

	friend class PhysicsWorld;
	friend class CollisionSolver;
};

