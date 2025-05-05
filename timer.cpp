#include "timer.h"

namespace Tmpl8
{

    Timer::Timer() : startingDuration(60), duration(startingDuration) {} // Initialize starting duration to 60

    void Timer::Start() {
        startTime = std::chrono::steady_clock::now(); // Current time is starting time of timer
    }
    // Function to update the time
    void Timer::Update() 
    { 
        auto endTime = std::chrono::steady_clock::now(); // Current time
        auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime); // Calculates time elapsed in seconds
        duration = startingDuration - elapsedSeconds.count(); // Calculates remaing duration
        if (duration < 0) { // Makes sure duration isn't negative
            duration = 0;
        }
    }
    // Checks if the timer is finished
    bool Timer::IsFinished() const 
    { 
        return duration <= 0;
    }

    // Gets the remaining duration 
    int Timer::GetDuration() const 
    {
        return duration;
    }

    // Function to reset timer
    void Timer::Reset() {
        duration = startingDuration; //  Resets duration equal to startingDuration (60 seconds)
        startTime = std::chrono::steady_clock::now();
    }
}