#ifndef TIMER_H
#define TIMER_H

//High-resolution timer
//Calculates time by Time::timestamp which uses std::chrono

//Usage:
// Timer t;
// t.start();
// printf("ms=%d\n", t.getTicks());
namespace Core
{
	class Timer
	{
	public:
		//Initializes variables
		Timer();
		~Timer();

		//The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();

		//Gets the timer's time as ms
		long getTicks();
		//Checks the status of the timer
		const bool& isStarted() const;
		const bool isPaused() const;


		static long timestamp();

	private:
		//The clock time when the timer started
		long startTicks;

		//The ticks stored when the timer was paused
		long pausedTicks;

		//The timer status
		bool paused;
		bool started;
	};
}

#endif