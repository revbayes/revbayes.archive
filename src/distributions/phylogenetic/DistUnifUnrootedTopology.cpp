/*
 * DistUnifUnrootedTopology.cpp
 *
 *  Created on: 25 aug 2009
 *      Author: Sebastian
 */

#include <cmath>
#include <cassert>
#include "ArgumentRule.h"
#include "DAGNode.h"
#include "DistUnifUnrootedTopology.h"
#include "RbUnrootedTopology.h"
#include "RbDouble.h"
#include "RbException.h"
#include "RbInt.h"
#include "RbMath.h"
#include "RbStatistics.h"
#include "RbUndefined.h"
#include "RbNames.h"


DistUnifUnrootedTopology::DistUnifUnrootedTopology(DAGNode* n, RandomNumberGenerator* r) : Distribution(NULL, r) {

	numTaxa = n;
//	returnType = RbNames::UnrootedTopology::name;
}

DistUnifUnrootedTopology::DistUnifUnrootedTopology(const DistUnifUnrootedTopology& d) : Distribution(d) {

	numTaxa = d.numTaxa;
//	returnType = d.returnType;
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
double DistUnifUnrootedTopology::pdf(const RbObject* obs) {
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
double DistUnifUnrootedTopology::lnPdf(const RbObject* obs) {

	int m = ((RbInt*) numTaxa->getValue())->getValue();
	RbUnrootedTopology* o = ((RbUnrootedTopology*) obs);

	double lnpdf = RbStatistics::UniformUnrootedTopology::lnPdf(m,o);

	return lnpdf;
}

double  DistUnifUnrootedTopology::lnPdfRatio(const RbObject* newVal, const RbObject* oldVal) {
    return 0.0;
}


RbObject* DistUnifUnrootedTopology::rv(void) {
	int m = ((RbInt*) numTaxa->getValue())->getValue();

	RbUnrootedTopology* u = RbStatistics::UniformUnrootedTopology::rv(m,rng);
	return (RbObject*)u;
}

const RbObject* DistUnifUnrootedTopology::executeOperation(const std::string& name, std::vector<DAGNode*>& args) {
    return NULL;
}

bool DistUnifUnrootedTopology::equals(const RbObject* o) const {

	return false;
}

/** Get class vector describing type of object */
const StringVector& DistUnifUnrootedTopology::getClass(void) const {

    static StringVector rbClass = StringVector(RbNames::DistUnifUnrootedTopology::name) + Distribution::getClass();
    return rbClass;
}

/** Get the argument rules */
const ArgumentRule** DistUnifUnrootedTopology::getMemberRules(void) {

    const static ArgumentRule* argRules[] = {
        new ArgumentRule( "taxa", RbNames::Int::name, new RbUndefined, new RbInt(1), new RbUndefined ),
        NULL };
    return argRules;
}

const std::string DistUnifUnrootedTopology::getReturnType(void) const {
    return RbNames::UnrootedTopology::name;
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
    
    return (*this);
}

