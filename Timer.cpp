//
// Created by adamc on 19/10/2023.
//


#include "Timer.h"

long long int Timer::readQPC(){
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count); //read number of ticks since the last restart of the system
    return((long long int)count.QuadPart); //64b integer
}

void Timer::startTimer(){
    QueryPerformanceFrequency((LARGE_INTEGER*)&frequency); //10MHz
    start = readQPC(); //number of ticks since the last restart of the system
}

long Timer::stopTimer(){
    elapsed = readQPC() - start; //read ticks once again and subtract from read ticks at the start

    long time = (1000000.0 * elapsed) / frequency;
    return time;
}

void Timer::printTime(){
    std::cout << "Time [s] = " << std::fixed << std::setprecision(6) << (float)elapsed / frequency << std::endl;
    std::cout << "Time [ms] = " << std::fixed << std::setprecision(4) << (1000.0 * elapsed) / frequency << std::endl;
    std::cout << "Time [us] = " << std::fixed << std::setprecision(1) << (1000000.0 * elapsed) / frequency << std::endl;
    std::cout << "Time [ns] = " << std::fixed << std::setprecision(0) << (1000000000.0 * elapsed) / frequency << std::endl << std::endl;
}

Timer::Timer() {

}