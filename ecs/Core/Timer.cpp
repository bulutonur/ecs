#include <ctime>
#include <chrono>
#include "Timer.h"

namespace Core
{
	Timer::Timer() :
		startTicks(0),
		pausedTicks(0),
		paused(false),
		started(false)
	{

	}

	Timer::~Timer()
	{

	}

	void Timer::start()
	{
		//Start the timer
		started = true;

		//Unpause the timer
		paused = false;

		//Get the current clock time
		startTicks = timestamp();
		pausedTicks = 0;
	}

	void Timer::stop()
	{
		//Stop the timer
		started = false;

		//Unpause the timer
		paused = false;

		//Clear tick variables
		startTicks = 0;
		pausedTicks = 0;
	}

	void Timer::pause()
	{
		//If the timer is running and isn't already paused
		if (started && !paused)
		{
			//Pause the timer
			paused = true;

			//Calculate the paused ticks
			pausedTicks = timestamp() - startTicks;
			startTicks = 0;
		}
	}

	void Timer::unpause()
	{
		//If the timer is running and paused
		if (started && paused)
		{
			//Unpause the timer
			paused = false;

			//Reset the starting ticks
			startTicks = timestamp() - pausedTicks;

			//Reset the paused ticks
			pausedTicks = 0;
		}
	}

	long Timer::getTicks()
	{
		//The actual timer time
		long time = 0;

		//If the timer is running
		if (started)
		{
			//If the timer is paused
			if (paused) {
				//Return the number of ticks when the timer was paused
				time = pausedTicks;
			}
			else {
				//Return the current time minus the start time
				time = timestamp() - startTicks;

			}
		}

		return time;
	}

	const bool& Timer::isStarted() const
	{
		//Timer is running and paused or unpaused
		return started;
	}

	const bool Timer::isPaused() const
	{
		//Timer is running and paused
		return paused && started;
	}

	long Timer::timestamp()
	{
		auto now = std::chrono::high_resolution_clock::now();
		auto duration = now.time_since_epoch();
		return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	}

}
