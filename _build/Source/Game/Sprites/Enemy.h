#pragma once
#include "Sprite.h"
#include "Game/Physics/Rigidbody.h"

class Enemy : public Sprite 
{
public:
	Enemy(Type type, Vector2 position, Vector2 size, Color color = WHITE);
	~Enemy();

	void SetName(const string& name) { mName = name; }
	void ImGuiDraw();
private:
	Rigidbody mRigidbody;
	string mName;
	float mSpeed;
	float mNegativeSpeed;
	bool mFacingLeft;
	void FixedUpdate(const float timeStep);

	std::function<void(const float)> mFixedUpdate;
};

