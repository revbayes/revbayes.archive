/*
 * DistNormal.cpp
 *
 *  Created on: 25 aug 2009
 *      Author: Sebastian
 */

#include <cmath>
#include <cassert>

#include "DAGNode.h"
#include "DistUniform.h"
#include "RbDouble.h"
#include "RbException.h"
#include "RbMath.h"
#include "RbStatistics.h"
#include "RbNames.h"

const StringVector DistUniform::rbClass = StringVector(RbNames::Uniform::name) + Distribution::rbClass;

DistUniform::DistUniform(DAGNode* l, DAGNode* u, RandomNumberGenerator* r) : Distribution(r) {

	lower = l;
	upper = u;
	returnType = RbNames::Double::name;
}

DistUniform::DistUniform(const DistUniform& d) : Distribution(d) {

	lower = d.lower;
	upper = d.upper;
	returnType = d.returnType;
}

DistUniform::~DistUniform() {
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
RbObject* DistUniform::clone(void) const {
	return new DistUniform(*this);
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
double DistUniform::pdf(const RbObject* obs) {
	// first some argument checking
	assert(typeid(*obs) == typeid(RbDouble));

	double x = ((RbDouble*) obs)->getValue();
    double l = ((RbDouble*) lower->getValue())->getValue();
    double u = ((RbDouble*) upper->getValue())->getValue();
	double pdf = RbStatistics::Uniform::pdf(l, u, x);

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
double DistUniform::lnPdf(const RbObject* obs) {
	// first some argument checking
	assert(typeid(*obs) == typeid(RbDouble));

    double x = ((RbDouble*) obs)->getValue();
    double l = ((RbDouble*) lower->getValue())->getValue();
    double u = ((RbDouble*) upper->getValue())->getValue();
	return RbStatistics::Normal::lnPdf(l,u,x);
}

RbObject* DistUniform::rv() {
    double lw = ((RbDouble*) lower->getValue())->getValue();
    double up = ((RbDouble*) upper->getValue())->getValue();
    double u = RbStatistics::Uniform::rv(lw, up, rng);
	RbDouble* x = new RbDouble(u);
	return (RbObject*)x;
}

bool DistUniform::equals(const RbObject* o) const {

	return false;
}

const StringVector& DistUniform::getClass(void) const {
    return rbClass;
}

void DistUniform::print(std::ostream& o) const {

	o << "Uniform Distribution" << std::endl;
}

void DistUniform::printValue(std::ostream& o) const {

	o << "Uniform Distribution with some value that we won't give" << std::endl;
}

std::string DistUniform::toString(void) const {

	return "Uniform Distribution(" + lower->toString() + "," + upper->toString() + ")";
}

RbObject& DistUniform::operator=(const RbObject& obj) {


    try {
        // Use built-in fast down-casting first
        const DistUniform& x = dynamic_cast<const DistUniform&> (obj);

        DistUniform& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const DistUniform& x = dynamic_cast<const DistUniform&> (*(obj.convertTo(RbNames::Uniform::name)));

            DistUniform& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to " + RbNames::Uniform::name);
            throw e;
        }
    }

    // dummy return
    return (*this);
}

DistUniform& DistUniform::operator=(const DistUniform& obj) {

    *lower = *(obj.lower);
    *upper = *(obj.upper);
    
    return (*this);
}

