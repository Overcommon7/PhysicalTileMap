#include "pch.h"
#include "InputHandling.h"

void InputHandling::Update(Controls& controls, const Keys& keys)
{
	controls.mIsHoldingJump = IsDown(keys.jump);
	controls.mIsHoldingLeft = IsDown(keys.left);
	controls.mIsHoldingRight = IsDown(keys.right);
	controls.mIsHoldingRun = IsDown(keys.run);

	controls.mThisFrame.jumpPressed = IsPressedThisFrame(keys.jump);
	controls.mThisFrame.leftPressed = IsPressedThisFrame(keys.left);
	controls.mThisFrame.rightPressed = IsPressedThisFrame(keys.right);

	controls.mThisFrame.jumpReleased = IsReleasedThisFrame(keys.jump);
	controls.mThisFrame.leftReleased = IsReleasedThisFrame(keys.left);
	controls.mThisFrame.rightReleased = IsReleasedThisFrame(keys.right);
}

bool InputHandling::IsPressedThisFrame(const std::array<KeyboardKey, 2>& keys)
{
	for (auto key : keys)
		if (IsKeyPressed(key))
			return true;

	return false;
}

bool InputHandling::IsReleasedThisFrame(const std::array<KeyboardKey, 2>& keys)
{
	for (auto key : keys)
		if (IsKeyReleased(key))
			return true;

	return false;
}

bool InputHandling::IsDown(const std::array<KeyboardKey, 2>& keys)
{
	for (auto key : keys)
		if (IsKeyDown(key))
			return true;

	return false;
}
