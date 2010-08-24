/**
 * @file
 * This file contains the implementation of Dist_multinomial, which is used to hold
 * parameters and functions related to a multinomial distribution.
 *
 * @brief Implementation of Dist_multinomial
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
#include "Dist_multinomial.h"
#include "Move_mmultinomial.h"
#include "RealPos.h"
#include "RandomNumberGenerator.h"
#include "Real.h"
#include "RbException.h"
#include "RbMath.h"
#include "RbNames.h"
#include "RbStatistics.h"
#include "Simplex.h"
#include "VectorInteger.h"
#include "VectorReal.h"
#include "VectorString.h"
#include "Workspace.h"
#include "WrapperRule.h"

#include <cmath>
#include <sstream>
#include <vector>


/** Default constructor for parser use */
Dist_multinomial::Dist_multinomial(void) : Distribution(getMemberRules()) {

}

/** Constructor for internal use */
Dist_multinomial::Dist_multinomial(std::vector<double> a) : Distribution(getMemberRules()) {

    setValue("p", new Simplex(a));
}

/** Clone this object */
Dist_multinomial* Dist_multinomial::clone(void) const {

    return new Dist_multinomial(*this);
}

/** Get class vector showing type of object */
const VectorString& Dist_multinomial::getClass(void) const {

    static VectorString rbClass = VectorString(Dist_multinomial_name) + Distribution::getClass();
    return rbClass;
}

/** Get default move */
Move* Dist_multinomial::getDefaultMove(StochasticNode* node) {

	// default move for a stochastic node having a Dirichlet distribution
    return new Move_mmultinomial(node, 300.0, 4, 1.0);
}

/** Get min value of distribution */
const Real* Dist_multinomial::getMin(void) {

    return NULL;
}

/** Get member variable rules */
const MemberRules& Dist_multinomial::getMemberRules(void) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) 
		{
        memberRules.push_back(new WrapperRule("p", Simplex_name));

        /* Inherit rng from Distribution, put it at back */
        const MemberRules& inheritedRules = Distribution::getMemberRules();
        memberRules.insert(memberRules.end(), inheritedRules.begin(), inheritedRules.end()); 

        rulesSet = true;
		}

    return memberRules;
}

/** Get random variable type */
const std::string& Dist_multinomial::getVariableType(void) const {

    return VectorRealPos_name;
}

/**
 * This function calculates the natural log of the likelihood
 * ratio for an multinomially-distributed random variable under
 * two different values of the distribution parameter.
 *
 * @brief Natural log of multinomial likelihood ratio
 *
 * @param value     Value of random variable
 * @return          Natural log of the likelihood ratio
 */
double Dist_multinomial::lnLikelihoodRatio(const RbObject* value) {

	// get the value and the parameters of the Multinomial
    std::vector<double> pNew = ((Simplex*) (getVariable("p")->getValue      ()))->getValue();
    std::vector<double> pOld = ((Simplex*) (getVariable("p")->getStoredValue()))->getValue();
    std::vector<double> x    = ((VectorReal*) value)->getValue();
	
	// check that the vectors are all the same size
	if ( pNew.size() != pOld.size() || pNew.size() != x.size() )
		throw (RbException("Inconsistent size of vectors when calculating Multinomial likelihood ratio"));
	
	// calculate the likelihood ratio for the two values of the Multinomial parameters
	double lnP = 0.0;
	for (size_t i=0; i<x.size(); i++)
        lnP += x[i] * ( std::log(pNew[i]) - std::log(pOld[i]));
	return lnP;	
}

/**
 * This function calculates the natural log of the probability
 * density for an multinomially-distributed random variable.
 *
 * @brief Natural log of multinomial probability density
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
double Dist_multinomial::lnPdf(const RbObject* value) {

	// get the value and the parameters of the Dirichlet
    std::vector<double> p = ((Simplex*) getValue("p"))->getValue();
    std::vector<double> x = ((VectorReal*) value)->getValue();

	// check that the vectors are both the same size
	if ( p.size() != x.size() )
		throw (RbException("Inconsistent size of vectors when calculating Dirichlet log probability density"));

	return RbStatistics::Multinomial::lnPdf( p, x );
}

/**
 * This function calculates the natural log of the probability
 * density ratio for two exponentially-distributed random variables.
 *
 * @brief Natural log of exponential probability density ratio
 *
 * @param newX      Value in numerator
 * @param oldX      Value in denominator
 * @return          Natural log of the probability density ratio
 */
double Dist_multinomial::lnPriorRatio(const RbObject* newVal, const RbObject* oldVal) {

	// get the values and the parameters of the Dirichlet
    std::vector<double> p    = ((Simplex*) getValue("p"))->getValue();
    std::vector<double> newX = ((VectorReal*) newVal)->getValue();
    std::vector<double> oldX = ((VectorReal*) oldVal)->getValue();

	// check that the vectors are all the same size
	if ( p.size() != newX.size() || p.size() != oldX.size() )
		throw (RbException("Inconsistent size of vectors when calculating Dirichlet prior ratio"));

	// calculate the log prior ratio
	double lnP = RbStatistics::Multinomial::lnPdf( p, newX ) - RbStatistics::Multinomial::lnPdf( p, oldX );
    return lnP;
}

/**
 * This function calculates the probability
 * for an multinomially-distributed random variable.
 *
 * @brief Multinomial probability
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_multinomial::pdf(const RbObject* value) {

	// get the value and the parameters of the Dirichlet
    std::vector<double> p = ((Simplex*) getValue("p"))->getValue();
    std::vector<double> x = ((VectorReal*) value)->getValue();

	// check that the vectors are both the same size
	if ( p.size() != x.size() )
		throw (RbException("Inconsistent size of vectors when calculating Dirichlet log probability density"));

	return RbStatistics::Multinomial::pdf( p, x );
}

/**
 * This function generates an multinomial-distributed
 * random variable.
 *
 * @brief Random draw from multinomial distribution
 *
 * @return      Random draw from multinomial distribution
 */
RbObject* Dist_multinomial::rv(void) {

    std::vector<double> p      = ((Simplex*) getValue("p"))->getValue();
    RandomNumberGenerator* rng = GLOBAL_RNG;
	std::vector<int> r(p.size());

	r = RbStatistics::Multinomial::rv(p, rng);
    return new VectorInteger( r );
}


