/**
 * @file
 * This file contains the implementation of RandomNumberGenerator, which is
 * used to generate uniform(0,1) random variables.
 *
 * @brief Declaration of RandomNumberGenerator
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#include "RandomNumberGenerator.h"
#include "RbException.h"
#include <ctime>

using namespace RevBayesCore;

/** Default constructor calling time to get the initial seeds */
RandomNumberGenerator::RandomNumberGenerator(void) :
    seed((unsigned int)( time(0) ) ),
    rng(),
    zeroone(rng)
{

	unsigned int x  = (unsigned int)( time(0) );    
    rng.seed( x );
    zeroone = boost::uniform_01<boost::rand48>(rng);

}


/* Get the seed values */
unsigned int RandomNumberGenerator::getSeed( void ) const
{
    return seed;
}


/** Set the seed of the random number generator */
void RandomNumberGenerator::setSeed(unsigned int s) {

    seed = s;
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
    return zeroone();
}
