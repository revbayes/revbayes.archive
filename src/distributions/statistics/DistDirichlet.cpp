/*
 * DistNormal.cpp
 *
 *  Created on: 25 aug 2009
 *      Author: Sebastian
 */

#include <cmath>
#include <cassert>
#include "DAGNode.h"
#include "DistDirichlet.h"
#include "RbDouble.h"
#include "RbException.h"
#include "RbMath.h"
#include "RbStatistics.h"
#include "RbNames.h"
#include "Vector.h"


DistDirichlet::DistDirichlet(DAGNode* a, RandomNumberGenerator* r) : Distribution(r) {

    alpha = a;
    value = NULL;
    returnType = RbNames::Vector::name;
}

DistDirichlet::DistDirichlet(const DistDirichlet& d) : Distribution(d) {

    *alpha = *d.alpha;
    *value    = *d.value;
    returnType = d.returnType;
}

DistDirichlet::~DistDirichlet() {
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
RbObject* DistDirichlet::clone(void) const {
    return new DistDirichlet(*this);
}

/*!
 * This function calculates the probability density
 * for a Dirichletly-distributed random variable.
 *
 * \brief Dirichlet probability density.
 * \param mu is the mean parameter of the Dirichlet.
 * \param sigma is the variance parameter of the Dirichlet.
 * \param x is the Dirichlet random variable.
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double DistDirichlet::pdf(const RbObject* obs) {
    std::vector<double>& a = ((Vector*) alpha->getValue())->getValue();
    std::vector<double>& o = ((Vector*) obs)->getValue();

    double pdf = RbStatistics::Dirichlet::pdf(a,o);

    return pdf;
}

/*!
 * This function calculates the natural log of the probability density
 * for a Dirichletly-distributed random variable.
 *
 * \brief Natural log of Dirichlet probability density.
 * \param mu is the mean parameter of the Dirichlet.
 * \param sigma is the variance parameter of the Dirichlet.
 * \param x is the Dirichlet random variable.
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double DistDirichlet::lnPdf(const RbObject* obs) {
    std::vector<double>& a = ((Vector*) alpha->getValue())->getValue();
    std::vector<double>& o = ((Vector*) obs)->getValue();

    double lnpdf = RbStatistics::Dirichlet::lnPdf(a,o);

    return lnpdf;
}

RbObject* DistDirichlet::rv(void) {
    std::vector<double>& a = ((Vector*) alpha->getValue())->getValue();

    std::vector<double> u = RbStatistics::Dirichlet::rv(a, rng);
    Vector* x = new Vector(u);
    return (RbObject*)x;
}

bool DistDirichlet::equals(const RbObject* o) const {

    return false;
}

/** Get string showing inheritance */
const StringVector& DistDirichlet::getClass(void) const {

    static StringVector rbClass = StringVector(RbNames::Dirichlet::name) + Distribution::getClass();
    return rbClass;
}

void DistDirichlet::print(std::ostream& o) const {

    o << "Dirichlet Distribution" << std::endl;
}

void DistDirichlet::printValue(std::ostream& o) const {

    o << "Dirichlet Distribution with some value that we won't give" << std::endl;
}

std::string DistDirichlet::toString(void) const {

    return "Dirichlet Distribution( alpha = " + alpha->toString() + ")";
}

DistDirichlet& DistDirichlet::operator=(const DistDirichlet& obj) {

    *alpha = *(obj.alpha);
    *value = *(obj.value);

    return (*this);
}

