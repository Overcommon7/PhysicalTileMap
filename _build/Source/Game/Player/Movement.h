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
		struct Debug
		{
			bool applyForce = true;
		};

		float maxWalkingSpeed = 500;
		float maxRunningSpeed = 500;
		float walkingAcclerationSpeed = 50;
		float runningAcclerationSpeed = 75;
		float walkingDecclerationSpeed = 45;
		float runningDecclerationSpeed = 60;
		float decclerationSpeed = 35;
		float initialJumpForce = 600;
		float maintainedJumpForce = 50;
		float maxAirTime = 0.45f;
		

		float timeOfJump = 0.f; 
		bool canStartJump = false;
		bool isJumpActive = false;

		Direction facing = Direction::Right;
		Direction moving = Direction::None;
		Debug mDebug;

		Values() = default;
		Values(const fs::path& path);
		void SaveToFile();


	private:
		const fs::path& mPath = "Debug/MovementValues.txt";
	};
public:
	static void Update(Player* player, Values& values);
	static void FixedUpdate(Player* player, Values& values, const float timeStep);
	static void ImGuiDraw(Values& values);
private:
	static void UpdateHoriantalMovement(Player* player, Values& values, const float timeStep);
	static void ValidateJump(Player* player, Values& values);
	static void DoJump(Player* player, Values& values, float fixedTimeStep);
};

