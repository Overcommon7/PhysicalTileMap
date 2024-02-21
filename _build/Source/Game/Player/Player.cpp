#include "pch.h"
#include "Player.h"

Player::Player(Vector2 position, Vector2 size)
	: Sprite(Sprite::Type::Player, position, size, PINK)
	, mRigidbody(this, PhysicsLayer::PLAYER)
{

}

void Player::Update()
{
	InputHandling::Update(mControls, mKeys);

}
