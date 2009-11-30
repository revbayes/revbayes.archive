/*
 * DistNormal.cpp
 *
 *  Created on: 25 aug 2009
 *      Author: Sebastian
 */

#include <cmath>

#include "DAGNode.h"
#include "DistExponential.h"
#include "RbDouble.h"
#include "RbException.h"
#include "RbMath.h"
#include "RbStatistics.h"
#include "RbNames.h"

const StringVector DistExponential::rbClass = StringVector(RbNames::Exponential::name) + Distribution::rbClass;

DistExponential::DistExponential(DAGNode* l) {

	lambda = l;
	returnType = RbNames::Double::name;
}

DistExponential::DistExponential(const DistExponential& d) {

	lambda = d.lambda;
	returnType = d.returnType;
}

DistExponential::~DistExponential() {
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
RbObject* DistExponential::clone(void) const {
    DistExponential* x = new DistExponential(*this);
	return (RbObject*) x;
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
double DistExponential::pdf(RbObject* obs) {
    // first some argument checking
    assert(typeid(*obs) == typeid(RbDouble));

    double x = ((RbDouble*) obs)->getValue();
    double l = ((RbDouble*) lambda->getValue())->getValue();
	double pdf = RbStatistics::Exponential::pdf(l,x);

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
double DistExponential::lnPdf(RbObject* obs) {
    // first some argument checking
    assert(typeid(*obs) == typeid(RbDouble));

    double x = ((RbDouble*) obs)->getValue();
    double l = ((RbDouble*) lambda->getValue())->getValue();
	return RbStatistics::Exponential::lnPdf(l, x);
}

bool DistExponential::equals(const RbObject* o) const {

	return false;
}

const StringVector& DistExponential::getClass(void) const {
    rbClass;
}

void DistExponential::print(std::ostream& o) const {

	o << "Exponential Distrbibution" << std::endl;
}

void DistExponential::printValue(std::ostream& o) const {

	o << "Exponential Distribution with some value that we won't give" << std::endl;
}

std::string DistExponential::toString(void) const {

	return "Exponential Distribution(rate = " + lambda->toString() + ")";
}

RbObject& DistExponential::operator=(const RbObject& obj) {


    try {
        // Use built-in fast down-casting first
        const DistExponential& x = dynamic_cast<const DistExponential&> (obj);

        DistExponential& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const DistExponential& x = dynamic_cast<const DistExponential&> (*(obj.convertTo(RbNames::Exponential::name)));

            DistExponential& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to " + RbNames::Exponential::name);
            throw e;
        }
    }

    // dummy return
    return (*this);
}

DistExponential& DistExponential::operator=(const DistExponential& obj) {

    *lambda = *(obj.lambda);
    
    return (*this);
}

