#include "Timer.h"

Timer::Timer() {
	start = std::chrono::high_resolution_clock::now();
}

double Timer::getDuration() {
	std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - this->start;

	return duration.count();
}