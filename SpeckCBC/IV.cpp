/*
    This file uses the mt19937 Mersenne Twister algorithm with current time
    to generate random 32-bit integer numbers for Initialisation Vector
*/

#include <iostream>
#include <random>
#include <ctime>   // time() function
#include <cmath>

using namespace std;

unsigned long long IV()
{
	mt19937 rand_generator;
	rand_generator.seed(time(0)); //random seeding using the current time, recommended!
	uniform_int_distribution<unsigned long long>  rand_distribution(0, pow(2,32)-1);
	return rand_distribution(rand_generator);
}
