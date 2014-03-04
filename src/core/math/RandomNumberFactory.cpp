/**
 * @file
 * This file contains the implementation of RandomNumberFactory, which is
 * used to manage random number generating objects. The class has a pool
 * of random number objects that it can hand off as needed. This singleton
 * class has two seeds it manages: one is a global seed and the other is
 * a so called local seed.
 *
 * @brief Declaration of RandomNumberFactory
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

#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbOptions.h"
#include <ctime>
#include <climits>
#include <climits>

using namespace RevBayesCore;

/** Default constructor */
RandomNumberFactory::RandomNumberFactory(void) {

    setSeed();
    std::vector<unsigned int> s;
    s.push_back(I1);
    s.push_back(I2);
    seedGenerator = new RandomNumberGenerator(s);
}


/** Destructor */
RandomNumberFactory::~RandomNumberFactory(void) {

    delete seedGenerator;
}


/** Delete a random number object (remove it from the pool too) */
void RandomNumberFactory::deleteRandomNumberGenerator(RandomNumberGenerator* r) {

    allocatedRandomNumbers.erase( r );
    
    delete r;
}


/** Return a pointer to a random number object */
RandomNumberGenerator* RandomNumberFactory::getRandomNumberGenerator(void) {

    unsigned int s1 = (unsigned int)(seedGenerator->uniform01()*UINT_MAX);
    unsigned int s2 = (unsigned int)(seedGenerator->uniform01()*UINT_MAX);

    std::vector<unsigned int> s;
    s.push_back(s1);
    s.push_back(s2);
    RandomNumberGenerator* r = new RandomNumberGenerator( s );
    allocatedRandomNumbers.insert( r );
    return r;
}


/** Return a pointer to a random number object with a specific set of seeds */
RandomNumberGenerator* RandomNumberFactory::getRandomNumberGenerator(std::vector<unsigned int> s) {

    if (s.size() < 2)
        throw(RbException("Problem returning random number generator: Too few seeds."));
    if (s.size() > 2)
        throw(RbException("Problem returning random number generator: Too many seeds."));
    RandomNumberGenerator* r = new RandomNumberGenerator( s );
    allocatedRandomNumbers.insert( r );
    return r;
}


/** Set the seeds for the factory using the current time */
void RandomNumberFactory::setSeed(void) {

// Define DEBUG_RANDOM to cause deterministic behavior
#if defined DEBUG_RANDOM
    setSeed(1);
    return;
#endif

    unsigned int x = (unsigned int)( time( 0 ) );
    I1 = x & 0xFFFF;
    I2 = x >> 16;
}

void RandomNumberFactory::setSeed(int s) {
    I1 = s & 0xFFFF;
    I2 = s >> 16;
}
