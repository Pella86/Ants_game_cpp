/*
 *  Random_numbers.cpp
 *  Yeasts_cpp
 *
 *  Created by Mauro Pellanda on 23.11.11.
 *  Copyright 2011 ETH. All rights reserved.
 *
 */

#include "Random_numbers.h"

#define MAX_FAST_RAND 79228162514264337593543950330.


Random_number::Random_number(){
	uno_su_rand_max = 1/double(RAND_MAX);
	x=123456789; 
	y=362436069;
	z=521288629;
	seed();
}

int Random_number::get_rand(){
	return std::rand();
}


inline double Random_number::get(){
	return (double)std::rand()/(double)(RAND_MAX);
}

double Random_number::get(double a, double b){
	return (b-a)*this->get()+a;
}

long Random_number::get(long n){
	if (n<0) n = -n; // converte in positivo il numero
	if (n == 0) return 0;
	long guard = (long) (get()*n)+1; // genera in numero random tra (0 e n) +1
	return (guard > n)? n : guard; // se il numero generato e' n+1 allora restitusce n
}

void Random_number::seed(){
	//reset the generator
	std::srand(5050505);
}

int Random_number::get_int_prob(float x){
	return ((float)RAND_MAX*x);
}

/*
double Random_number::get(){//period 2^96-1
		unsigned long t;
		x ^= x << 16;
		x ^= x >> 5;
		x ^= x << 1;
		
		t = x;
		x = y;
		y = z;
		z = t ^ x ^ y;
		
		return z/(double)MAX_FAST_RAND;
}*/