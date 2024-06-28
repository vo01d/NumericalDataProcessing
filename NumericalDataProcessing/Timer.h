#ifndef _TIMER_H_
#define _TIMER_H_

#include <chrono>

class Timer {
public:
	Timer();
	double getDuration();
private:
	std::chrono::time_point<std::chrono::steady_clock> start;
};

#endif