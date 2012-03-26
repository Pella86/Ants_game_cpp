/*
 *  timer.cpp
 *  AntsGame
 *
 *  Created by Mauro Pellanda on 12.03.12.
 *  Copyright 2012 UZH. All rights reserved.
 *
 */

#include "timer.h"

bool Timer::elasped(clock_t ms){
	clock_t tick = std::clock();
	if (tick - counter >= ms){
		counter = tick;
		return true;
	}
	return false;
}