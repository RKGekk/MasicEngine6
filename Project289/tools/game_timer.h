#pragma once

#include <chrono>

using gameClock = std::chrono::steady_clock;
using gameClockDuration = gameClock::duration;
using gameTimePoint = gameClock::time_point;

const gameClockDuration ZERO_DURATION = gameClockDuration(0);

class GameTimer {
public:
	GameTimer();

	float TotalTime() const; // in seconds
	float DeltaTime() const; // in seconds

	gameClockDuration TotalTimeDuration() const; // in seconds
	gameClockDuration DeltaTimeDuration() const; // in seconds

	void Reset(); // Call before message loop.
	void Start(); // Call when unpaused.
	void Stop();  // Call when paused.
	void Tick();  // Call every frame.

private:
	gameClockDuration mDeltaTimeDuration;

	gameTimePoint mBaseTime;
	gameClockDuration mPausedTime;
	gameTimePoint mStopTime;
	gameTimePoint mPrevTime;
	gameTimePoint mCurrTime;

	bool mStopped;
};