#pragma once
struct Controls
{
	struct ThisFrame
	{
		bool rightPressed;
		bool rightReleased;
		bool leftPressed;
		bool leftReleased;
		bool jumpPressed;
		bool jumpReleased;	
	};

	bool isHoldingRight;
	bool isHoldingLeft;
	bool isHoldingJump;

	ThisFrame thisFrame;
};

struct Keys
{
	KeyboardKey jump;
	KeyboardKey left;
	KeyboardKey right; 
};

struct InputHandling
{
	static void Update(Controls& controls, const Keys& keys);
};

