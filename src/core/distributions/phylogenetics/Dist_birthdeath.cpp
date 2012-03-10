/**
 * @file
 * This file contains the implementation of Dist_birthdeath
 *
 * @brief Implementation of Dist_birthdeath
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-07-26, version 1.0
 *
 * $Id$
 */

#include "RbBoolean.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "Dist_birthdeath.h"
#include "Integer.h"
#include "Natural.h"
#include "Probability.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TreePlate.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "ValueRule.h"
#include "Variable.h"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <vector>

/** Default constructor for parser use */
Dist_birthdeath::Dist_birthdeath( void ) : Distribution( getMemberRules() ),
                                            origin( NULL ),
                                            presentTime( NULL ),
                                            lambda( NULL ),
                                            mu( NULL ),
                                            rho( NULL ),
                                            speciationEvent( NULL ) {
    
}


/** Clone this object */
Dist_birthdeath* Dist_birthdeath::clone( void ) const {
    
    return new Dist_birthdeath( *this );
}


/** Get class name of object */
const std::string& Dist_birthdeath::getClassName(void) { 
    
    static std::string rbClassName = "Birth-Death distribution";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Dist_birthdeath::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Dist_birthdeath::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get member variable rules */
const MemberRules& Dist_birthdeath::getMemberRules( void ) const {
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
    
    if ( !rulesSet )
    {
        memberRules.push_back( new ValueRule( "origin"          , RealPos::getClassTypeSpec()     ) );
        memberRules.push_back( new ValueRule( "T"               , RealPos::getClassTypeSpec()     ) );
        memberRules.push_back( new ValueRule( "lambda"          , RealPos::getClassTypeSpec()     ) );
        memberRules.push_back( new ValueRule( "mu"              , RealPos::getClassTypeSpec(), new RealPos(0.0) ) );
        memberRules.push_back( new ValueRule( "rho"             , Probability::getClassTypeSpec(), new Probability(1.0) ) );
        memberRules.push_back( new ValueRule( "speciationEvent" , RbBoolean::getClassTypeSpec(), new RbBoolean(true) ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/** Get random variable type */
const TypeSpec& Dist_birthdeath::getVariableType( void ) const {
    
    static TypeSpec varTypeSpec = RealPos::getClassTypeSpec();
    
    return varTypeSpec;
}


/**
 * This function calculates the natural log of the
 * probability of a random variable.
 *
 * @brief Natural log of probability
 *
 * @param value Observed speciation times
 * @return      Natural log of the probability
 */
double Dist_birthdeath::lnPdf( const RbLanguageObject& value ) const {
    
    // Get the parameters
    double t = static_cast<const RealPos&    >( value                   ).getValue();
    double o = static_cast<const RealPos&    >( origin->getValue()       ).getValue();
    double T = static_cast<const RealPos&    >( presentTime->getValue()  ).getValue();
    double b = static_cast<const RealPos&    >( lambda->getValue()       ).getValue();
    double d = static_cast<const RealPos&    >( mu->getValue()           ).getValue();
    double p = static_cast<const Probability&>( rho->getValue()          ).getValue();
    
    // have we observed a speciation event at time t or did we just stop the process without oberving an event?
    // Internal nodes correspond to obsereved speciation events whereas tips correspond to no event and a stopped process.
    bool event = static_cast<const RbBoolean&>( speciationEvent->getValue() ).getValue();
    
    // the probability of the current time is the probability of having observed no event until now
    double log_p = log( pWaiting(o,t,T,b,d,p) );
    
    // if this is for an observed speciation event and not only that we haven't observed an event until now
    // then we multiply with the probability of a speciation event which results into a surviving and sampled species.
    if ( event ) {
        log_p += log( pBirth(t,T,b,d,p) );
    }
    
    return log_p;
}


double Dist_birthdeath::nj(double t, double t_prime, double T, double lambda, double mu, double rho) const {
    return ut(t,t_prime,lambda,mu,rho) * pSurvival(t,T,lambda,mu,rho) / pSurvival(t,t_prime,lambda,mu,rho);
}


double Dist_birthdeath::pBirth(double t, double T, double lambda, double mu, double rho) const {
    return pSurvival(t,T,lambda,mu,rho) * lambda; 
}


/**
 * This function calculates the probability
 * of a categorical random variable.
 *
 * @brief Categorical probability density
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_birthdeath::pdf( const RbLanguageObject& value ) const {
    
    // Get the parameters
    double t = static_cast<const RealPos&    >( value                    ).getValue();
    double o = static_cast<const RealPos&    >( origin->getValue()       ).getValue();
    double T = static_cast<const RealPos&    >( presentTime->getValue()  ).getValue();
    double b = static_cast<const RealPos&    >( lambda->getValue()       ).getValue();
    double d = static_cast<const RealPos&    >( mu->getValue()           ).getValue();
    double p = static_cast<const Probability&>( rho->getValue()          ).getValue();
    
    // have we observed a speciation event at time t or did we just stop the process without oberving an event?
    // Internal nodes correspond to obsereved speciation events whereas tips correspond to no event and a stopped process.
    bool event = static_cast<const RbBoolean&>( speciationEvent->getValue() ).getValue();
    
    // the probability of the current time is the probability of having observed no event until now
    double prob = pWaiting(o,t,T,b,d,p);
    
    // if this is for an observed speciation event and not only that we haven't observed an event until now
    // then we multiply with the probability of a speciation event which results into a surviving and sampled species.
    if ( event ) {
        prob *= pBirth(t,T,b,d,p);
    }
    
    return prob;
}



double Dist_birthdeath::pSurvival(double start, double end, double lambda, double mu, double rho) const {
    
    double xxx = ( exp( (mu - lambda) * (end - start) ) - 1.0 ) / (mu - lambda);
    double den = 1.0 + mu * ( xxx ) - ( rho - 1.0 ) * exp( rate(start,end,lambda,mu,rho) );
    
    return (1.0 / den);
}



double Dist_birthdeath::pWaiting(double start, double observed, double end, double lambda, double mu, double rho) const {
    return 1.0 - nj(start,observed,end,lambda,mu,rho);
}


double Dist_birthdeath::rate(double start, double end, double lambda, double mu, double rho) const {
    
    // birth-death with constant rate
    return (mu - lambda) * (end - start);
}



double Dist_birthdeath::ut(double t, double t_prime, double lambda, double mu, double rho) const {
    return 1.0 - pSurvival(t,t_prime,lambda,mu,rho) * exp( rate(t,t_prime,lambda,mu,rho) );
}


/**
 * This function generates a vector of speciation times from the distribution
 *
 * @brief Random draw from distribution
 *
 * @return      Randomly drawn vector of speciation times
 */
const RbLanguageObject& Dist_birthdeath::rv( void ) {
    
    // TODO needs implementation!!!
    
    // Get the parameters
    double o = static_cast<const RealPos&    >( origin->getValue()       ).getValue();
    double T = static_cast<const RealPos&    >( presentTime->getValue()  ).getValue();
    double b = static_cast<const RealPos&    >( lambda->getValue()       ).getValue();
    double d = static_cast<const RealPos&    >( mu->getValue()           ).getValue();
    double p = static_cast<const Probability&>( rho->getValue()          ).getValue();
    
    randomVariable.setValue( (T - o) / 2.0 + o );
    
    return randomVariable;
}


