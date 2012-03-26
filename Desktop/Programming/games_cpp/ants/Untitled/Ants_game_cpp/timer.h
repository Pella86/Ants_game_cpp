/*
 *  timer.h
 *  AntsGame
 *
 *  Created by Mauro Pellanda on 12.03.12.
 *  Copyright 2012 UZH. All rights reserved.
 *
 */

#ifndef TIMER_H
#define TIMER_H

#include <ctime>

class Timer {
private:
	clock_t counter;
public:
	Timer():counter(0) {}; //c'tor
	bool elasped(clock_t ms);
};

#endif //TIMER_H