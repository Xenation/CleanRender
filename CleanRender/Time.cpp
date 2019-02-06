#include "Time.h"

using namespace std::chrono;



float Time::time = 0.f;
float Time::deltaTime = 0.f;

high_resolution_clock::time_point Time::startTime = high_resolution_clock::now();
high_resolution_clock::time_point Time::currentTime = high_resolution_clock::now();
duration<float> Time::durationSinceStart;
duration<float> Time::durationFrame;

void Time::ComputeFrameTimes() {
	high_resolution_clock::time_point nTime = high_resolution_clock::now();
	durationSinceStart = duration_cast<duration<float>>(nTime - startTime);
	durationFrame = duration_cast<duration<float>>(nTime - currentTime);
	currentTime = nTime;
	time = durationSinceStart.count();
	deltaTime = durationFrame.count();
}
