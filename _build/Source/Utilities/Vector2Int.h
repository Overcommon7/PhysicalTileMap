#pragma once

struct Vector2Int final
{
	int x, y;
	Vector2Int() : x(0), y(0) {}
	Vector2Int(int x, int y) : x(x), y(y) {}
	Vector2Int(float x, float y) : x((int)x), y ((int)y) {}
	Vector2Int(const Vector2& vec) : x((int)vec.x), y ((int)vec.y) {}
	Vector2Int(const ImVec2& vec) : x((int)vec.x), y ((int)vec.y) {}

	Vector2Int (const Vector2Int& vec) = default;
	Vector2Int& operator=(const Vector2Int& vec) = default;

	Vector2Int& operator=(const Vector2& vec) noexcept
	{
		x = (int)vec.x;
		y = (int)vec.y;

		return *this;
	}

	Vector2Int& operator=(const ImVec2& vec) noexcept
	{
		x = (int)vec.x;
		y = (int)vec.y;

		return *this;
	}

	Vector2Int operator+(const Vector2Int& vec) const
	{
		return { x + vec.x, y + vec.y };
	}
	Vector2Int operator-(const Vector2Int& vec) const
	{
		return { x - vec.x, y - vec.y };
	}
	Vector2Int operator*(const Vector2Int& vec) const
	{
		return { x * vec.x, y * vec.y };
	}
	Vector2Int operator/(const Vector2Int& vec) const
	{
		return { x / vec.x, y / vec.y };
	}
	Vector2Int operator+(const Vector2& vec) const
	{
		return { x + vec.x, y + vec.y };
	}
	Vector2Int operator-(const Vector2& vec) const
	{
		return { x - vec.x, y - vec.y };
	}
	Vector2Int operator*(const Vector2& vec) const
	{
		return { x * vec.x, y * vec.y };
	}
	Vector2Int operator/(const Vector2& vec) const
	{
		return { x / vec.x, y / vec.y };
	}
	Vector2Int operator+(const int s) const
	{
		return { x + s, y + s };
	}
	Vector2Int operator-(const int s) const
	{
		return { x - s, y - s };
	}
	Vector2Int operator*(const int s) const
	{
		return { x * s, y * s };
	}
	Vector2Int operator/(const float s) const
	{
		return { x / s, y / s };
	}
	Vector2Int operator+(const float s) const
	{
		return { x + s, y + s };
	}
	Vector2Int operator-(const float s) const
	{
		return { x - s, y - s };
	}
	Vector2Int operator*(const float s) const
	{
		return { x * s, y * s };
	}
	friend Vector2Int operator+(const Vector2& vec, const Vector2Int& iVec)
	{
		return { iVec.x + vec.x, iVec.y + vec.y };
	}
	friend Vector2Int operator-(const Vector2& vec, const Vector2Int& iVec)
	{
		return { iVec.x - vec.x, iVec.y - vec.y };
	}
	friend Vector2Int operator*(const Vector2& vec, const Vector2Int& iVec)
	{
		return { iVec.x * vec.x, iVec.y * vec.y };
	}
	friend Vector2Int operator/(const Vector2& vec, const Vector2Int& iVec)
	{
		return { vec.x / iVec.x, vec.y / iVec.y };
	} 

	Vector2Int Negate() const
	{
		return { -x, -y };
	}

	Vector2Int Abs() const
	{
		return { abs(x), abs(y) };
	}

	int SqrMagintude() const
	{
		return (x * x) + (y * y);
	}

	float Magnitude() const
	{
		return (float)sqrt(SqrMagintude());
	}

	operator Vector2() const
	{
		return Vector2((float)x, (float)y);
	}

	operator ImVec2() const
	{
		return ImVec2(float(x), float(y));
	}

	bool operator==(const Vector2Int& other) const {
		return x == other.x && y == other.y;
	}

	bool operator<(const Vector2Int& other) const {
		if (x == other.x)
			return y < other.y;
		return x < other.x;
	}
};


