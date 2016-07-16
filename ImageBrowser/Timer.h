#pragma once
#include <cstdint>

class Timer
{
public:
	Timer();

	void Start();
	void Stop();
	void Pause();
	void Unpause();

	uint32_t GetTicks();

	bool IsStarted();
	bool IsPaused();

private:
	uint32_t mStartTicks;
	uint32_t mPausedTicks;

	bool mPaused;
	bool mStarted;
};