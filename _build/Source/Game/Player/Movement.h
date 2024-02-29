#pragma once
class Player;

struct Movement
{
	enum class Direction
	{
		Left,
		Right,
		None
	};
	struct Values
	{
		float maxSpeed = 500;
		float walkingAcclerationSpeed = 50;
		float runningAcclerationSpeed = 75;
		float walkingDecclerationSpeed = 45;
		float runningDecclerationSpeed = 60;
		float decclerationSpeed = 35;
		float initialJumpForce = 600;
		float maxAirTime = 3.1f;
		

		float timeOfJump = 0.f; 
		bool canStartJump = false;
		bool isJumpActive = false;

		Direction facing = Direction::Right;
		Direction moving = Direction::None;
	};
public:
	static void Update(Player* player, Values& values, const float timeStep);
	static void ImGuiDraw(Values& values);
private:
	static void Move(Player* player, Values& values, const float timeStep);
	static void Jump(Player* player, Values& values, const float timeStep);

	static void TurnAround(Player* player, Values& values, const float timeStep);
	static void ApplyHorizantalForce(Player* player, Values& values, const float timeStep);

	static void ValidateJump(Player* player, Values& values, const float timeStep);
	static void ApplyVerticalForce(Player* player, Values& values, const float timeStep);

	
};

