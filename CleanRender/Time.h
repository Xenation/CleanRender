#pragma once
#include <chrono>
class Time {
public:
	static float time;
	static float deltaTime;

	static void ComputeFrameTimes();

	Time() = delete;
	Time(const Time&) = delete;
	~Time() = delete;

private:
	static std::chrono::high_resolution_clock::time_point startTime;
	static std::chrono::high_resolution_clock::time_point currentTime;
	static std::chrono::duration<float> durationSinceStart;
	static std::chrono::duration<float> durationFrame;
};

