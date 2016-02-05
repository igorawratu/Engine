#include "timer.h"
#include <iostream>

Timer::Timer(){
    total_time_elapsed_ms_ = SDL_GetTicks();
    actual_time_elapsed_ms_ = 0;
    paused_time_ms_ = 0;
    time_since_last_frame_ms_ = 0;

    total_time_elapsed_ = toSeconds(total_time_elapsed_ms_);
    actual_time_elapsed_ = 0.f;
    paused_time_ = 0.f;
    time_since_last_frame_ = 0.f;

    paused_ = false;
}

Timer::Timer(const Timer& other){
    total_time_elapsed_ms_ = other.total_time_elapsed_ms_;
    actual_time_elapsed_ms_ = other.actual_time_elapsed_ms_;
    paused_time_ms_ = other.paused_time_ms_;
    time_since_last_frame_ms_ = other.time_since_last_frame_ms_;

    total_time_elapsed_ = other.total_time_elapsed_;
    actual_time_elapsed_ = other.actual_time_elapsed_;
    paused_time_ = other.paused_time_;
    time_since_last_frame_ = other.time_since_last_frame_;

    paused_ = other.paused_;
}

Timer::~Timer(){

}

float Timer::toSeconds(int ms){
    return ((float)ms / 1000.f);
}

float Timer::deltaTime(){
    return time_since_last_frame_;
}

float Timer::totalTime(){
    return total_time_elapsed_;
}

float Timer::timePaused(){
    return paused_time_;
}

float Timer::actualTime(){
    return actual_time_elapsed_;
}

void Timer::pause(){
    paused_ = true;
}

void Timer::unpause(){
    paused_ = false;
    paused_time_ms_ = 0;
    paused_time_ = 0.f;
}

bool Timer::isPaused(){
    return paused_;
}

void Timer::frame(){
    int new_tick_time = SDL_GetTicks();
    int diff = new_tick_time - actual_time_elapsed_;
    actual_time_elapsed_ = new_tick_time;

    if(paused_){
        paused_time_ms_ += diff;
        paused_time_ = toSeconds(paused_time_ms_);
        time_since_last_frame_ms_ = 0;
    }
    else{
        total_time_elapsed_ms_ += diff;
        time_since_last_frame_ms_ = diff;

        total_time_elapsed_ = toSeconds(total_time_elapsed_ms_);
    }

    time_since_last_frame_ = toSeconds(time_since_last_frame_ms_);
}

void Timer::reset(){
    total_time_elapsed_ms_ = SDL_GetTicks();
    actual_time_elapsed_ms_ = 0;
    paused_time_ms_ = 0;
    time_since_last_frame_ms_ = 0;

    total_time_elapsed_ = toSeconds(total_time_elapsed_ms_);
    actual_time_elapsed_ = 0.f;
    paused_time_ = 0.f;
    time_since_last_frame_ = 0.f;

    paused_ = false;
}
