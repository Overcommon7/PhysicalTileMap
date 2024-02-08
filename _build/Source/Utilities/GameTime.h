#pragma once

class Time final
{
public:
	inline static const float Now() { return now; }
	inline static const float DeltaTime() { return dt; }

	friend class App;
private:
	inline static float now = 0;
	inline static float dt = 0;

	inline static void Update()
	{
		dt = GetFrameTime();
		now += dt;
	}	
};

