/*
 * DistUnifUnrootedTopology.cpp
 *
 *  Created on: 25 aug 2009
 *      Author: Sebastian
 */

#include <cmath>
#include <cassert>
#include "DAGNode.h"
#include "DistUnifUnrootedTopology.h"
#include "RbUnrootedTopology.h"
#include "RbDouble.h"
#include "RbException.h"
#include "RbInt.h"
#include "RbMath.h"
#include "RbStatistics.h"
#include "RbNames.h"


const StringVector DistUnifUnrootedTopology::rbClass = StringVector(RbNames::DistUnifUnrootedTopology::name) + Distribution::rbClass;

DistUnifUnrootedTopology::DistUnifUnrootedTopology(DAGNode* n, RandomNumberGenerator* r) : Distribution(r) {

	numTaxa = n;
	returnType = RbNames::UnrootedTopology::name;
}

DistUnifUnrootedTopology::DistUnifUnrootedTopology(const DistUnifUnrootedTopology& d) : Distribution(d) {

	numTaxa = d.numTaxa;
	returnType = d.returnType;
}

DistUnifUnrootedTopology::~DistUnifUnrootedTopology() {
	// TODO Auto-generated destructor stub
}

/**
 * @brief Copy this object
 *
 * This is a call of the copy constructor used from the base class
 *
 * @return     return a deep copy of the object
 *
 */
RbObject* DistUnifUnrootedTopology::clone(void) const {
	return new DistUnifUnrootedTopology(*this);
}

/*!
 * This function calculates the probability density
 * for a normally-distributed random variable.
 *
 * \brief Normal probability density.
 * \param mu is the mean parameter of the normal.
 * \param sigma is the variance parameter of the normal.
 * \param x is the normal random variable.
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double DistUnifUnrootedTopology::pdf(RbObject* obs) {
	int m = ((RbInt*) numTaxa->getValue())->getValue();
	RbUnrootedTopology* o = ((RbUnrootedTopology*) obs);

	double pdf = RbStatistics::UniformUnrootedTopology::pdf(m,o);

	return pdf;
}

/*!
 * This function calculates the natural log of the probability density
 * for a normally-distributed random variable.
 *
 * \brief Natural log of normal probability density.
 * \param mu is the mean parameter of the normal.
 * \param sigma is the variance parameter of the normal.
 * \param x is the normal random variable.
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double DistUnifUnrootedTopology::lnPdf(RbObject* obs) {

	int m = ((RbInt*) numTaxa->getValue())->getValue();
	RbUnrootedTopology* o = ((RbUnrootedTopology*) obs);

	double lnpdf = RbStatistics::UniformUnrootedTopology::lnPdf(m,o);

	return lnpdf;
}

RbObject* DistUnifUnrootedTopology::rv(void) {
	int m = ((RbInt*) numTaxa->getValue())->getValue();

	RbUnrootedTopology* u = RbStatistics::UniformUnrootedTopology::rv(rng,m);
	return (RbObject*)u;
}

bool DistUnifUnrootedTopology::equals(const RbObject* o) const {

	return false;
}

const StringVector& DistUnifUnrootedTopology::getClass(void) const {
    rbClass;
}

void DistUnifUnrootedTopology::printValue(std::ostream& o) const {

	o << "Uniform distribution of unrooted topologies with some value that we won't give" << std::endl;
}

std::string DistUnifUnrootedTopology::toString(void) const {

	return "Uniform distribution of unrooted tree topologies ( taxa = " + numTaxa->toString() + ")";
}

RbObject& DistUnifUnrootedTopology::operator=(const RbObject& obj) {


    try {
        // Use built-in fast down-casting first
        const DistUnifUnrootedTopology& x = dynamic_cast<const DistUnifUnrootedTopology&> (obj);

        DistUnifUnrootedTopology& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const DistUnifUnrootedTopology& x = dynamic_cast<const DistUnifUnrootedTopology&> (*(obj.convertTo(RbNames::DistUnifUnrootedTopology::name)));

            DistUnifUnrootedTopology& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to " + RbNames::DistUnifUnrootedTopology::name);
            throw e;
        }
    }

    // dummy return
    return (*this);
}

DistUnifUnrootedTopology& DistUnifUnrootedTopology::operator=(const DistUnifUnrootedTopology& obj) {

    *numTaxa = *(obj.numTaxa);
    returnType = obj.returnType;
    
    return (*this);
}

