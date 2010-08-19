

#include "RandomNumberGenerator.h"
#include "RbComplex.h"
#include "RbException.h"
#include "RbObject.h"
#include "RbNames.h"
#include <ctime>


/** Default constructor calling time to get the initial seeds */
RandomNumberGenerator::RandomNumberGenerator(void)
    : RbComplex() {

	unsigned int x  = (unsigned int)( time(0) );
	unsigned int s1 = x & 0xFFFF;
	unsigned int s2 = x >> 16;
	seed.push_back(s1);
	seed.push_back(s2);
}


/** Constructor explicitly setting the seeds */
RandomNumberGenerator::RandomNumberGenerator(std::vector<unsigned int> s) {

    if (s.size() != 2)
        throw RbException("Two integer seeds required");

	seed = s;
}


/** Clone function */
RbObject* RandomNumberGenerator::clone(void) const {

    return new RandomNumberGenerator(*this);
}


/** Convert to object of another class. The caller manages the object. */
RbObject* RandomNumberGenerator::convertTo(const std::string& type) const {

    throw (RbException("Conversion of " + getType() + " to " + type + " not supported"));
    return NULL;
}


/** Pointer-based equals comparison */
bool RandomNumberGenerator::equals(const RbObject* obj) const {

    const RandomNumberGenerator* p = dynamic_cast<const RandomNumberGenerator*>(obj);
    if (p == NULL)
        return false;

    if (seed != p->seed)
        return false;

    return true;
}


/** Get class vector describing type of object */
const VectorString& RandomNumberGenerator::getClass(void) const { 

    static VectorString rbClass = VectorString(RandomNumberGenerator_name) + RbComplex::getClass();
	return rbClass;
}


/** Is convertible to type? */
bool RandomNumberGenerator::isConvertibleTo(const std::string& type) const {

    return false;
}


/** Print value for user */
void RandomNumberGenerator::printValue(std::ostream& o) const {

    o << "Random number generator (seed = " << VectorInteger(seed) << ")";
}


/** Get complete info about object */
std::string RandomNumberGenerator::toString(void) const {

    std::ostringstream o;
    o << "Random Number Generator:" << std::endl;
    o << "seed = " << VectorInteger(seed) << std::endl;

    return o.str();
}


/** Set the seed of the random number generator */
void RandomNumberGenerator::setSeed(std::vector<unsigned int> s) {

    if (s.size() != 2)
        throw RbException("Two integer seeds required");

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
	seed[0] = 36969 * (seed[0] & 0177777) + (seed[0] >> 16);
	seed[1] = 18000 * (seed[1] & 0177777) + (seed[1] >> 16);
	return (((seed[0] << 16)^(seed[1] & 0177777)) * 2.328306437080797e-10) + 2.328306435996595e-10;
}
