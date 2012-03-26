/*
 *  Random_numbers.h
 *  Yeasts_cpp
 *
 *  Created by Mauro Pellanda on 23.11.11.
 *  Copyright 2011 ETH. All rights reserved.
 *
 */

#ifndef RANDOM_NUMBER_H
#define RANDOM_NUMBER_H

#include <iostream>
#include <ctime>

#define print(X) std::cout<<X<<std::endl


class Random_number{
private:
	double uno_su_rand_max;
	unsigned long long x,y,z;
public:
	Random_number();
	double get();
	int get_rand();
	double get(double a, double b);
	long get(long n);
	void seed();
	int get_int_prob(float x);
};

#endif
