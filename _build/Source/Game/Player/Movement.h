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

		float maxWalkingSpeed = 325;
		float maxRunningSpeed = 450;
		float walkingAcclerationSpeed = 15;
		float runningAcclerationSpeed = 35;
		float walkingDecclerationSpeed = 935;
		float runningDecclerationSpeed = 1350;
		float passiveDecclerationSpeed = 750;
		float initialJumpForce = 350;
		float maintainedJumpForce = 45;
		float maxAirTime = 0.45f;
		float maxHeightBoost = 0.2f;

		
		
		float jumpHeightBoost = 0.f;
		float timeOfJump = 0.f; 
		bool canStartJump = false;
		bool isJumpActive = false;

		Direction facing = Direction::Right;
		Direction moving = Direction::None;

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
	static void ApplyAcceleration(Player* player, Values& values, const float timeSte);
	static void ApplyDecceleration(Player* player, Values& values, const float timeSte);
	static void ValidateJump(Player* player, Values& values);
	static void DoJump(Player* player, Values& values, float fixedTimeStep);
};

