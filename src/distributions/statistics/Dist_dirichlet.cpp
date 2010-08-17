/**
 * @file
 * This file contains the implementation of Dist_exp, which is used to hold
 * parameters and functions related to an exponential distribution.
 *
 * @brief Implementation of Dist_exp
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
#include "Dist_dirichlet.h"
#include "Move_mscale.h"
#include "PosReal.h"
#include "RandomNumberGenerator.h"
#include "RbDouble.h"
#include "RbException.h"
#include "RbMath.h"
#include "RbNames.h"
#include "StringVector.h"
#include "Vector.h"
#include "Workspace.h"
#include "WrapperRule.h"

#include <cmath>
#include <sstream>
#include <vector>


/** Default constructor for parser use */
Dist_dirichlet::Dist_dirichlet(void) : DistributionReal(getMemberRules()) {

}

/** Constructor for internal use */
Dist_dirichlet::Dist_dirichlet(std::vector<double> a, RandomNumberGenerator* rng) : DistributionReal(getMemberRules()) {

    setValue("alpha", new Vector(a));
    setValue("rng",  rng);
}

/**
 * This function calculates the cumulative probability for
 * an exponentially-distributed random variable.
 *
 * @brief Exponential cumulative probability
 *
 * @param q     Quantile
 * @return      Cumulative probability
 *
 */
double Dist_dirichlet::cdf(double q) {

	/* TO DO: We should implement the cumulative probability for the Dirichlet. The most recent
	   algorithms are discussed in
	   
	   Gouda, A. A., and T. Szantai. 2010. On numerical calculation of probabilities according 
	   to Dirichlet distribution. Ann. Oper. Res. 177:185–200. */
    return 0.0;
}

/** Clone this object */
Dist_dirichlet* Dist_dirichlet::clone(void) const {

    return new Dist_dirichlet(*this);
}

/** Get class vector showing type of object */
const StringVector& Dist_dirichlet::getClass(void) const {

    static StringVector rbClass = StringVector(Dist_dirichlet_name) + DistributionReal::getClass();
    return rbClass;
}

/** Get default move */
Move* Dist_dirichlet::getDefaultMove(StochasticNode* node) {

    return new Move_mscale(node, 2.0*std::log(1.5), 1.0, Workspace::globalWorkspace().get_rng());
}

/** Get min value of distribution */
const RbDouble* Dist_dirichlet::getMin(void) {

    static RbDouble rbZero = RbDouble(0.0);
    return &rbZero;
}

/** Get member variable rules */
const MemberRules& Dist_dirichlet::getMemberRules(void) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) 
		{
        memberRules.push_back(new WrapperRule("alpha", Vector_name));

        /* Inherit rng from Distribution, put it at back */
        const MemberRules& inheritedRules = Distribution::getMemberRules();
        memberRules.insert(memberRules.end(), inheritedRules.begin(), inheritedRules.end()); 

        rulesSet = true;
		}

    return memberRules;
}

/** Get random variable type */
const std::string& Dist_dirichlet::getVariableType(void) const {

    return Vector_name;
}

/**
 * This function calculates the natural log of the likelihood
 * ratio for an Dirichlet-distributed random variable under
 * two different values of the distribution parameter.
 *
 * @brief Natural log of exponential likelihood ratio
 *
 * @param value     Value of random variable
 * @return          Natural log of the likelihood ratio
 */
double Dist_dirichlet::lnLikelihoodRatio(const RbObject* value) {

	// get the value and the parameters of the Dirichlet
    std::vector<double> aNew = ((Vector*) (getVariable("alpha")->getValue      ()))->getValue();
    std::vector<double> aOld = ((Vector*) (getVariable("alpha")->getStoredValue()))->getValue();
    std::vector<double> x    = ((Vector*) value)->getValue();
	
	// check that the vectors are all the same size
	if ( aNew.size() != aOld.size() || aNew.size() != x.size() )
		throw (RbException("Inconsistent size of vectors when calculating Dirichlet likelihood ratio"));
	
	// calculate the likelihood ratio for the two values of the Dirichlet parameters
	int n = aNew.size();
	double alpha0New = 0.0, alpha0Old = 0.0;
	for (int i=0; i<n; i++)
		{
		alpha0New += aNew[i];
		alpha0Old += aOld[i];
		}
	double lnP = RbMath::lnGamma(alpha0New) - RbMath::lnGamma(alpha0Old);
	for (int i=0; i<n; i++)
		lnP += ( RbMath::lnGamma(aOld[i]) - RbMath::lnGamma(aNew[i]) ) + ( (aNew[i] - aOld[i]) * std::log(x[i]) );
	return lnP;	
}

/**
 * This function calculates the natural log of the probability
 * density for an exponentially-distributed random variable.
 *
 * @brief Natural log of exponential probability density
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
double Dist_dirichlet::lnPdf(const RbObject* value) {

	// get the value and the parameters of the Dirichlet
    std::vector<double> a = ((Vector*) getValue("alpha"))->getValue();
    std::vector<double> x = ((Vector*) value)->getValue();

	// check that the vectors are both the same size
	if ( a.size() != x.size() )
		throw (RbException("Inconsistent size of vectors when calculating Dirichlet log probability density"));

	// calculate the log probability density
	int n = a.size();
	double alpha0 = 0.0;
	for (int i=0; i<n; i++)
		alpha0 += a[i];
	double lnP = RbMath::lnGamma(alpha0);
	for (int i=0; i<n; i++)
		lnP -= RbMath::lnGamma(a[i]);
	for (int i=0; i<n; i++)
		lnP += (a[i] - 1.0) * std::log(x[i]);	
    return lnP;
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
double Dist_dirichlet::lnPriorRatio(const RbObject* newVal, const RbObject* oldVal) {

	// get the values and the parameters of the Dirichlet
    std::vector<double> a    = ((Vector*) getValue("alpha"))->getValue();
    std::vector<double> newX = ((Vector*) newVal)->getValue();
    std::vector<double> oldX = ((Vector*) oldVal)->getValue();

	// check that the vectors are all the same size
	if ( a.size() != newX.size() || a.size() != oldX.size() )
		throw (RbException("Inconsistent size of vectors when calculating Dirichlet prior ratio"));

	// calculate the log prior ratio
	int n = a.size();
	double lnP = 0.0;
	for (int i=0; i<n; i++)
		lnP += (a[i] - 1.0) * (std::log(newX[i]) - std::log(oldX[i]));
    return lnP;
}

/**
 * This function calculates the probability density
 * for an exponentially-distributed random variable.
 *
 * @brief Exponential probability density
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_dirichlet::pdf(const RbObject* value) {

	double lnP = lnP(value);
	if (lnP < -300.0)
		return 0.0;
    return exp(lnP);
}

/**
 * This function calculates the quantile for an
 * exponentially-distributed random variable.
 *
 * @brief Quantile of exponential probability density
 *
 * @param p     Cumulative probability of quantile
 * @return      Quantile
 *
 */
double Dist_dirichlet::quantile(const double p) {

	throw (RbException("The quanitle function is not implemented for the Dirichlet distribution"));
    return 0.0;
}

/**
 * This function generates an exponentially-distributed
 * random variable.
 *
 * @brief Random draw from exponential distribution
 *
 * @return      Random draw from exponential distribution
 */
RbDouble* Dist_dirichlet::rv(void) {

    double                 lambda = ((RbDouble*) getValue("rate"))->getValue();
    RandomNumberGenerator* rng    = (RandomNumberGenerator*)(getValue("rng"));

    double u = rng->rv01();

    return new PosReal(-( 1.0 / lambda ) * std::log( u ));
}


