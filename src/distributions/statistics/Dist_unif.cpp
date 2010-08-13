/**
 * @file
 * This file contains the implementation of Dist_unif, which is used to hold
 * parameters and functions related to a uniform distribution.
 *
 * @brief Implementation of Dist_unif
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-14 12:43:32 +0100 (Mån, 14 Dec 2009) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z ronquist $
 */

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "Dist_unif.h"
#include "Move_mslide.h"
#include "PosReal.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbDouble.h"
#include "RbException.h"
#include "RbNames.h"
#include "StringVector.h"
#include "Workspace.h"
#include "WrapperRule.h"

#include <cmath>
#include <sstream>


/** Default constructor for parser use */
Dist_unif::Dist_unif(void)
    : DistributionReal(getMemberRules()) {
}


/** Constructor for test use */
Dist_unif::Dist_unif(double min, double max, RandomNumberGenerator* rng)
    : DistributionReal(getMemberRules()) {

    setValue("min", new RbDouble(min));
    setValue("max", new RbDouble(max));
    setValue("rng", rng);
}


/**
 * This function calculates the cumulative probability for
 * a uniformly-distributed random variable.
 *
 * @brief Exponential cumulative probability
 *
 * @param q     Quantile
 * @return      Cumulative probability
 *
 */
double Dist_unif::cdf(double q) {

    double min = ((RbDouble*) getValue("min"))->getValue();
    double max = ((RbDouble*) getValue("max"))->getValue();

    if ( q < min )
        return 0.0;
    else if ( q > max )
        return 1.0;

    return ( q - min ) / ( max - min );
}


/** Clone this object */
Dist_unif* Dist_unif::clone(void) const {

    return new Dist_unif(*this);
}


/** Get class vector showing type of object */
const StringVector& Dist_unif::getClass(void) const {

    static StringVector rbClass = StringVector(Dist_unif_name) + DistributionReal::getClass();
    return rbClass;
}


/** Get default move */
Move* Dist_unif::getDefaultMove(StochasticNode* node) {

    double min = ((RbDouble*) getValue("min"))->getValue();
    double max = ((RbDouble*) getValue("max"))->getValue();

    double delta = (max - min) / 100.0;

    return new Move_mslide(node, delta, 1.0, Workspace::globalWorkspace().get_rng());
}


/** Get max value of distribution */
const RbDouble* Dist_unif::getMax(void) {

    return (RbDouble*)(getValue("max"));
}


/** Get member variable rules */
const MemberRules& Dist_unif::getMemberRules(void) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) {

        memberRules.push_back(new WrapperRule("min", RbDouble_name));
        memberRules.push_back(new WrapperRule("max", RbDouble_name));

        /* Inherit rng from Distribution, put it at back */
        const MemberRules& inheritedRules = Distribution::getMemberRules();
        memberRules.insert(memberRules.end(), inheritedRules.begin(), inheritedRules.end()); 

        rulesSet = true;
    }

    return memberRules;
}


/** Get min value of distribution */
const RbDouble* Dist_unif::getMin(void) {

    return (RbDouble*)(getValue("min"));
}


/**
 * This function calculates the natural log of the likelihood
 * ratio for a uniformly-distributed random variable under
 * two different values of the distribution parameter.
 *
 * @brief Natural log of uniform likelihood ratio
 *
 * @param value     Value of random variable
 * @return          Natural log of the likelihood ratio
 */
double Dist_unif::lnLikelihoodRatio(const RbObject* value) {

    double minNew = ((RbDouble*) (getVariable("min")->getValue      ()))->getValue();
    double minOld = ((RbDouble*) (getVariable("min")->getStoredValue()))->getValue();
    double maxNew = ((RbDouble*) (getVariable("max")->getValue      ()))->getValue();
    double maxOld = ((RbDouble*) (getVariable("max")->getStoredValue()))->getValue();
    double x      = ((RbDouble*) value)->getValue();

    if ( x < minOld || x > maxOld )
        throw RbException("nan");

    if ( x < minNew || x > maxNew )
        return RbConstants::Double::neginf;

    return std::log( maxOld - minOld ) - std::log( maxNew - minNew );
}


/**
 * This function calculates the natural log of the probability
 * density for a uniformly-distributed random variable.
 *
 * @brief Natural log of uniform probability density
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
double Dist_unif::lnPdf(const RbObject* value) {

    double min = ((RbDouble*) getValue("min"))->getValue();
    double max = ((RbDouble*) getValue("max"))->getValue();
    double x   = ((RbDouble*) value)->getValue();

    if ( x < min || x > max )
        return RbConstants::Double::neginf;

    return -std::log( max - min );
}


/**
 * This function calculates the natural log of the probability
 * density ratio for two uniformly-distributed random variables.
 *
 * @brief Natural log of uniform probability density ratio
 *
 * @param newX      Value in numerator
 * @param oldX      Value in denominator
 * @return          Natural log of the probability density ratio
 */
double Dist_unif::lnPriorRatio(const RbObject* newVal, const RbObject* oldVal) {

    return 0.0;
}


/**
 * This function calculates the probability density
 * for a uniformly-distributed random variable.
 *
 * @brief Exponential probability density
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_unif::pdf(const RbObject* value) {

    double min = ((RbDouble*) getValue("min"))->getValue();
    double max = ((RbDouble*) getValue("max"))->getValue();
    double x   = ((RbDouble*) value)->getValue();

    if ( x < min || x > max )
        return 0.0;

    return 1.0 / ( max - min );
}


/**
 * This function calculates the quantile for an
 * uniformly-distributed random variable.
 *
 * @brief Quantile of uniform probability density
 *
 * @param p     Cumulative probability of quantile
 * @return      Quantile
 *
 */
double Dist_unif::quantile(const double p) {

    double min = ((RbDouble*) getValue("min"))->getValue();
    double max = ((RbDouble*) getValue("max"))->getValue();

    return min + ( max - min ) * p;
}


/**
 * This function generates a uniformly-distributed
 * random variable.
 *
 * @brief Random draw from uniform distribution
 *
 * @return      Random draw from uniform distribution
 */
RbDouble* Dist_unif::rv(void) {

    double                 min = ((RbDouble*) getValue("min"))->getValue();
    double                 max = ((RbDouble*) getValue("max"))->getValue();
    RandomNumberGenerator* rng = (RandomNumberGenerator*)(getValue("rng"));

    double u = rng->rv01();

    return new RbDouble(min + ( max - min ) * u);
}


