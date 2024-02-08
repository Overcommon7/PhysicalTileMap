#pragma once

#include "raylib.h"
#include "raymath.h"

#include "imgui.h"
#include "imgui_internal.h"

#include "rlImGui.h"

#include "Utilities/GameTime.h"
#include "Utilities/Vector2Int.h"
#include "Utilities/magic_enum.hpp"
#include "Utilities/Action.h"

#include <array>
#include <chrono>
#include <algorithm>
#include <any>
#include <atomic>
#include <bitset>
#include <charconv>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <random>
#include <ranges>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <concepts>
#include <execution>
#include <stdlib.h>

using std::cout;
using std::string;
using std::vector;
using std::list;
using std::fstream;
using std::ifstream;
using std::ostream;
using std::stringstream;
using std::ofstream;
using std::unordered_map;
using std::string_view;
using std::stringstream;
using std::function;
using std::pair;
using std::function;
using std::wstring;
using std::queue;
using std::set;
using std::unordered_set;
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;
using std::stoi;
using std::stof;
using std::stod;
using std::stold;
using std::stol;
using std::stoll;
using std::stoull;
using std::to_string;
using std::find;
using std::make_unique;

namespace ch = std::chrono;
namespace fs = std::filesystem;

static inline Color Convert(ImVec4 color)
{
	return {
		(unsigned char)std::clamp(color.x * 255.f, 0.f, 255.f),
		(unsigned char)std::clamp(color.y * 255.f, 0.f, 255.f),
		(unsigned char)std::clamp(color.z * 255.f, 0.f, 255.f),
		(unsigned char)std::clamp(color.w * 255.f, 0.f, 255.f)
	};
}

static inline ImVec4 Convert(Color color)
{
	return ImVec4(
		std::clamp<float>(color.r / 255.0f, 0, 1),
		std::clamp<float>(color.g / 255.0f, 0, 1),
		std::clamp<float>(color.b / 255.0f, 0, 1),
		std::clamp<float>(color.a / 255.0f, 0, 1));
}

struct Vector2IntHash
{
	size_t operator()(const Vector2Int& v) const
	{
		static std::hash<int> hasher{};
		size_t hashValue = hasher(v.x);
		hashValue ^= hasher(v.y) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
		return hashValue;
	}
};
