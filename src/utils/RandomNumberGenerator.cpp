#include "RandomNumberGenerator.h"
#include "RbComplex.h"
#include "RbException.h"
#include "RbObject.h"
#include <ctime>


const StringVector RandomNumberGenerator::rbClass = StringVector("rng") + RbComplex::rbClass;

RandomNumberGenerator::RandomNumberGenerator(std::string n, std::vector<unsigned int> s) {
    name = n;
    seed = s;
}

RandomNumberGenerator::RandomNumberGenerator(std::vector<unsigned int> s) {
	seed = s;
}

RandomNumberGenerator::RandomNumberGenerator(const RandomNumberGenerator& rng) {
    seed = rng.seed;
    name = rng.name;
}

RandomNumberGenerator::~RandomNumberGenerator() {

}

RbObject* RandomNumberGenerator::clone() const {
    RandomNumberGenerator* x = new RandomNumberGenerator(*this);
    return (RbObject*) x;
}

bool RandomNumberGenerator::equals(const RbObject* obj) const {
    return false;
}

void RandomNumberGenerator::print(std::ostream& o) const {
    o << "Random Number Generator (" << name << ")" << std::endl;
}

void RandomNumberGenerator::printValue(std::ostream& o) const {
    o << name << std::endl;
}
std::string RandomNumberGenerator::toString(void) const {
    return "Random Number Generator (" + name + ")";
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

RbObject& RandomNumberGenerator::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RandomNumberGenerator& x = dynamic_cast<const RandomNumberGenerator&> (obj);

        RandomNumberGenerator& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RandomNumberGenerator& x = dynamic_cast<const RandomNumberGenerator&> (*(obj.convertTo("rng")));

            RandomNumberGenerator& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to rng");
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RandomNumberGenerator& RandomNumberGenerator::operator=(const RandomNumberGenerator& obj) {

    seed = obj.seed;
    name = obj.name;
    return (*this);
}

void RandomNumberGenerator::setSeed(std::vector<unsigned int> s) {
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
	seed[1]= 18000 * (seed[1] & 0177777) + (seed[1] >> 16);
	return ((seed[0] << 16)^(seed[1] & 0177777)) * 2.328306437080797e-10; 	/*!< in [0,1) */
}
