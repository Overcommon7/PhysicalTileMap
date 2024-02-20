#pragma once

struct PhyicsLayer
{
private:
	inline static const std::hash<string> mHasher = {};
public:
	inline static const size_t PLAYER = mHasher("Player");

	inline static const std::array<size_t, 1> Layers = {
		PLAYER,
	};
};
