//
// Created by adamc on 19/10/2023.
//

#ifndef PEA1_TIMER_H
#define PEA1_TIMER_H

#include <windows.h>
#include <iostream>
#include <iomanip>

class Timer {
public:
    long long int frequency, start, elapsed;
    Timer();
    void startTimer();
    long stopTimer();
    void printTime();
    long long int readQPC();
};


#endif //PEA1_TIMER_H