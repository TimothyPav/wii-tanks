#ifndef TIMER_H
#define TIMER_H

#include <chrono> // for std::chrono functions
#include <string> // for std::string
#include <iomanip> // for std::setfill and std::setw
#include <sstream> // for std::stringstream

class Timer
{
private:
	// Type aliases to make accessing nested type easier
	using Clock = std::chrono::steady_clock;
	using Second = std::chrono::duration<double, std::ratio<1> >;

	std::chrono::time_point<Clock> m_beg { Clock::now() };

public:
	void reset()
	{
		m_beg = Clock::now();
	}

	double elapsed() const
	{
		return std::chrono::duration_cast<Second>(Clock::now() - m_beg).count();
	}

	std::string getFormattedTime() const
	{
		double totalSeconds = elapsed();
		
		// Extract minutes, seconds, and milliseconds
		int minutes = static_cast<int>(totalSeconds) / 60;
		int seconds = static_cast<int>(totalSeconds) % 60;
		int milliseconds = static_cast<int>((totalSeconds - static_cast<int>(totalSeconds)) * 1000);
		
		// Format as MM:SS:mmm
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(2) << minutes << ":"
		   << std::setfill('0') << std::setw(2) << seconds << ":"
		   << std::setfill('0') << std::setw(3) << milliseconds;
		
		return ss.str();
	}
};

#endif
