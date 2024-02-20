#include "pch.h"
#include "Player.h"

void Player::Update()
{
	InputHandling::Update(mControls, mKeys);

}
