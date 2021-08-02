#include "game_timer.h"

GameTimer::GameTimer() {
	mDeltaTimeDuration = ZERO_DURATION;
	gameTimePoint startTime = gameClock::now();
	mBaseTime = startTime;
	mPausedTime = ZERO_DURATION;
	mStopTime = startTime;
	mPrevTime = startTime;
	mCurrTime = startTime;
	mStopped = false;
}

gameClockDuration GameTimer::TotalTimeDuration() const {
	if (mStopped) {
		return mStopTime - mBaseTime - mPausedTime;
	}

	else {
		return mCurrTime - mBaseTime - mPausedTime;
	}
}

float GameTimer::TotalTime() const {
	return std::chrono::duration<float>(TotalTimeDuration()).count();
}

gameClockDuration GameTimer::DeltaTimeDuration() const {
	return mDeltaTimeDuration;
}

float GameTimer::DeltaTime() const {
	return std::chrono::duration<float>(mDeltaTimeDuration).count();
}

void GameTimer::Reset() {
	gameTimePoint startTime = gameClock::now();
	mBaseTime = startTime;
	mPrevTime = startTime;
	mStopTime = startTime;
	mStopped = false;
}

void GameTimer::Start() {
	gameTimePoint startTime = gameClock::now();

	if (mStopped) {
		mPausedTime += (startTime - mStopTime);

		mPrevTime = startTime;
		mStopTime = startTime;
		mStopped = false;
	}
	else {
		Reset();
	}
}

void GameTimer::Stop() {
	if (!mStopped) {
		mStopTime = gameClock::now();
		mStopped = true;
	}
}

void GameTimer::Tick() {
	if (mStopped) {
		mDeltaTimeDuration = ZERO_DURATION;
		return;
	}

	mCurrTime = gameClock::now();

	mDeltaTimeDuration = mCurrTime - mPrevTime;

	mPrevTime = mCurrTime;

	if (mDeltaTimeDuration < ZERO_DURATION) {
		mDeltaTimeDuration = ZERO_DURATION;
	}
}