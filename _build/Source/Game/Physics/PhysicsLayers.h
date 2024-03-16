#pragma once

struct PhysicsLayer
{
private:
	inline static const std::hash<string> mHasher = {};
public:
	inline static const size_t PLAYER = mHasher("Player");
	inline static const size_t CAMERA = mHasher("Camera");
	inline static const size_t ENEMIES = mHasher("Enemies");

	inline static const std::array<size_t, 3> Layers = {
		PLAYER,
		CAMERA,
		ENEMIES
	};
};
