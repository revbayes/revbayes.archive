#include "RandomNumberGenerator.h"
#include <ctime>



RandomNumberGenerator::RandomNumberGenerator(unsigned int x) {

	setSeed(x, 0);
}

RandomNumberGenerator::RandomNumberGenerator(unsigned int x, unsigned int y) {

	I1 = x;
	I2 = y;
}

int RandomNumberGenerator::nextInt(int max) {

	return (int)(uniform01()*max);
}

unsigned int RandomNumberGenerator::nextUnsignedInt(unsigned int max) {

	return (unsigned int)(uniform01()*max);
}

double RandomNumberGenerator::nextDouble(void) {

	return uniform01();
}

double RandomNumberGenerator::nextDouble(double max) {

	return ( uniform01()*max );
}

void RandomNumberGenerator::getSeed(unsigned int &seed1, unsigned int &seed2) {

	seed1 = I1;
	seed2 = I2;
}

void RandomNumberGenerator::setSeed(unsigned int seed1, unsigned int seed2) {

    if(seed2 == 0) 
		{
        I1 = seed1 & 0xFFFF;
        I2 = seed1 >> 16;
		}
	else 
		{
        I1 = seed1;
        I2 = seed2;
		}
}

/*!
 * This function generates a uniformly-distributed random variable on the interval [0,1).
 * It is a version of the Marsaglia Multi-Carry.
 *
 * Taken from:
 *   Mathlib : A C Library of Special Functions
 *   Copyright (C) 2000, 2003  The R Development Core Team
 *
 * This random generator has a period of 2^60, which ensures it has the maximum
 * period of 2^32 for unsigned ints (32 bit ints).
 *
 * \brief Uniform[0,1) random variable.
 * \return Returns a uniformly-distributed random variable on the interval [0,1).
 * \throws Does not throw an error.
 * \see http://stat.fsu.edu/~geo/diehard.html
 */
double RandomNumberGenerator::uniform01(void) {

	// Returns a pseudo-random number between 0 and 1.
	I1 = 36969 * (I1 & 0177777) + (I1 >> 16);
	I2 = 18000 * (I2 & 0177777) + (I2 >> 16);
	return ((I1 << 16)^(I2 & 0177777)) * 2.328306437080797e-10; 	/*!< in [0,1) */
}
