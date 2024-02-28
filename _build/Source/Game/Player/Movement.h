#pragma once
class Player;

struct Movement
{
	struct Values
	{
		float maxSpeed;
		float walkingAcclerationSpeed;
		float runningAcclerationSpeed;
		float initialJumpForce;
		float maxAirTime;
		

		float timeOfJump; 
		bool canJump;
		bool isJumpActive;
	};
public:
	static void Update(Player* player, Values& values, const float timeStep);
private:
	static void Move(Player* player, Values& values, const float timeStep);
	static void Jump(Player* player, Values& values, const float timeStep);

	static void TurnAround(Player* player, Values& values, const float timeStep);
	static void ApplyHorizantalForce(Player* player, Values& values, const float timeStep);

	static void ValidateJump(Player* player, Values& values, const float timeStep);
	static void ApplyVerticalForce(Player* player, Values& values, const float timeStep);
};

