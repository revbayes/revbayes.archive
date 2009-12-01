/*
 * DistNormal.cpp
 *
 *  Created on: 25 aug 2009
 *      Author: Sebastian
 */

#include <cmath>
#include <cassert>
#include "DAGNode.h"
#include "DistNormal.h"
#include "RbDouble.h"
#include "RbException.h"
#include "RbMath.h"
#include "RbStatistics.h"
#include "RbNames.h"


const StringVector DistNormal::rbClass = StringVector(RbNames::Normal::name) + Distribution::rbClass;

DistNormal::DistNormal(DAGNode* m, DAGNode* s) {

	sigma = s;
	mu    = m;
	returnType = RbNames::Double::name;
}

DistNormal::DistNormal(const DistNormal& d) {

	sigma = d.sigma;
	mu    = d.mu;
	returnType = d.returnType;
}

DistNormal::~DistNormal() {
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
RbObject* DistNormal::clone(void) const {
	return new DistNormal(*this);
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
double DistNormal::pdf(RbObject* obs) {
	double m = ((RbDouble*) mu->getValue())->getValue();
	double s = ((RbDouble*) sigma->getValue())->getValue();
	double o = ((RbDouble*) obs)->getValue();

	double pdf = RbStatistics::Normal::pdf(m,s,o);

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
double DistNormal::lnPdf(RbObject* obs) {

	double m = ((RbDouble*) mu->getValue())->getValue();
	double s = ((RbDouble*) sigma->getValue())->getValue();
	double o = ((RbDouble*) obs)->getValue();

	double lnpdf = RbStatistics::Normal::lnPdf(m,s,o);

	return lnpdf;
}

RbObject* DistNormal::rv(RandomNumberGenerator* r) {
	double m = ((RbDouble*) mu->getValue())->getValue();
	double s = ((RbDouble*) sigma->getValue())->getValue();

	double u = RbStatistics::Normal::rv(r,m,s);
	RbDouble* x = new RbDouble(u);
	return (RbObject*)x;
}

bool DistNormal::equals(const RbObject* o) const {

	return false;
}

const StringVector& DistNormal::getClass(void) const {
    rbClass;
}

void DistNormal::print(std::ostream& o) const {

	o << "Normal Distribution" << std::endl;
}

void DistNormal::printValue(std::ostream& o) const {

	o << "Normal Distribution with some value that we won't give" << std::endl;
}

std::string DistNormal::toString(void) const {

	return "Normal Distribution( mean = " + mu->toString() + ", std = " + sigma->toString() + ")";
}

RbObject& DistNormal::operator=(const RbObject& obj) {


    try {
        // Use built-in fast down-casting first
        const DistNormal& x = dynamic_cast<const DistNormal&> (obj);

        DistNormal& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const DistNormal& x = dynamic_cast<const DistNormal&> (*(obj.convertTo(RbNames::Normal::name)));

            DistNormal& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to " + RbNames::Normal::name);
            throw e;
        }
    }

    // dummy return
    return (*this);
}

DistNormal& DistNormal::operator=(const DistNormal& obj) {

    *mu = *(obj.mu);
    *sigma = *(obj.sigma);
    
    return (*this);
}

