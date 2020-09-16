#include "Timer.h"
#include "System.h"


Timer::Timer(float time)
{
    this->delay = timeToFrame(time);
    this->finish = false;
    this->start = false;
    this->ticks = 0;
}

//constructeur par defaut au cas où
Timer::Timer()
{
    this->delay = 0;
    this->finish = false;
    this->start = false;
    this->ticks = 0;
}

void Timer::launch(){
    this->start = true;
    this->finish = false;
    this->ticks = 0;
}

bool Timer::isLaunched(){
    return this->start;
}

void Timer::pass(){
    if(!this->finish && this->start){
        this->ticks++;
        if(this->ticks >= this->delay){
            this->finish = true;
        }
    }
}


float Timer::getTime(){
    return frameToTime(this->ticks);
}


float Timer::getRemainingTime(){
    return frameToTime(this->delay) - this->getTime();
}



bool Timer::isTicksMultipleBy(int multiple){
    return this->ticks%multiple == 0;
}

bool Timer::isTimeUnder(float time){
    return this->ticks < timeToFrame(time);
}

bool Timer::isFinished(){
    return this->finish;
}

Timer::~Timer()
{
    //dtor
}
