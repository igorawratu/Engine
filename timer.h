#ifndef TIMER
#define TIMER

#include <SDL.h>

class Timer{
private:
    int total_time_elapsed_ms_;
    int actual_time_elapsed_ms_;
    int paused_time_ms_;
    int time_since_last_frame_ms_;

    float total_time_elapsed_;
    float actual_time_elapsed_;
    float paused_time_;
    float time_since_last_frame_;

    bool paused_;

private:
    //forwards timer by a frame
    void frame();

    //helper for frame(), converts int ms value to float second value
    float toSeconds(int ms);

public:
    Timer();
    Timer(const Timer& other);
    ~Timer();

    /**
     * @brief Calculates the time since the last frame
     * @return the time in seconds
     */
    float deltaTime();

    /**
     * @brief Calculates the total amount of elapsed. This does not include pause time, and can be reset
     * @return the elapsed time in seconds
     */
    float totalTime();

    /**
     * @brief Calculates the total amount of time the current pause has elapsed
     * @return the elapsed time in seconds
     */
    float timePaused();

    /**
     * @brief Calculates the actual total time the system has run for, including pause times. This value is not reset when
     * reset() is called
     * @return the time in seconds
     */
    float actualTime();

    /**
     * @brief Pauses the timer
     */
    void pause();

    /**
     * @brief Unpauses the timer
     */
    void unpause();

    /**
     * @brief Checks if the timer is paused
     * @return
     */
    bool isPaused();

    /**
     * @brief Resets all the timers except the actual time elapsed. Also unpauses the timer.
     */
    void reset();
};

#endif // TIMER

