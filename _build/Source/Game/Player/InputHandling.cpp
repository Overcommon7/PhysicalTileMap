#include "pch.h"
#include "InputHandling.h"

void InputHandling::Update(Controls& controls, const Keys& keys)
{
	controls.isHoldingJump = IsKeyDown(keys.jump);
	controls.isHoldingLeft = IsKeyDown(keys.left);
	controls.isHoldingRight = IsKeyDown(keys.right);

	controls.thisFrame.jumpPressed = IsKeyPressed(keys.jump);
	controls.thisFrame.leftPressed = IsKeyPressed(keys.left);
	controls.thisFrame.rightPressed = IsKeyPressed(keys.right);

	controls.thisFrame.jumpReleased = IsKeyReleased(keys.jump);
	controls.thisFrame.leftReleased = IsKeyReleased(keys.left);
	controls.thisFrame.rightReleased = IsKeyReleased(keys.right);
}
