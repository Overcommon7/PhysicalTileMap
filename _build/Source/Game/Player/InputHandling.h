#pragma once
struct Controls
{
	struct ThisFrame
	{
		bool rightPressed = false;
		bool rightReleased = false;
		bool leftPressed = false;
		bool leftReleased = false;
		bool jumpPressed = false;
		bool jumpReleased = false;	
	};

	bool mIsHoldingRight = false;
	bool mIsHoldingLeft = false;
	bool mIsHoldingJump = false;

	ThisFrame mThisFrame{};
};

struct Keys
{
	std::array<KeyboardKey, 2> jump = { KEY_W, KEY_UP };
	std::array<KeyboardKey, 2> left = { KEY_A, KEY_LEFT };
	std::array<KeyboardKey, 2> right = { KEY_D, KEY_RIGHT }; 
};

struct InputHandling
{
	static void Update(Controls& controls, const Keys& keys);
private: 
	static bool IsPressedThisFrame(const std::array<KeyboardKey, 2>& keys);
	static bool IsReleasedThisFrame(const std::array<KeyboardKey, 2>& keys);
	static bool IsDown(const std::array<KeyboardKey, 2>& keys);
};

