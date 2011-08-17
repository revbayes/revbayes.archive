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
#include "DAGNode.h"
#include "Dist_birthdeath.h"
#include "Integer.h"
#include "Natural.h"
#include "Probability.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbNames.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "VectorRealPos.h"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <vector>


/** Default constructor for parser use */
Dist_birthdeath::Dist_birthdeath( void ) : Distribution( getMemberRules() ) {
    
}


/** Clone this object */
Dist_birthdeath* Dist_birthdeath::clone( void ) const {
    
    return new Dist_birthdeath( *this );
}


/** Get class vector showing type of object */
const VectorString& Dist_birthdeath::getClass( void ) const {
    
    static VectorString rbClass = VectorString( Dist_birthdeath_name ) + Distribution::getClass();
    return rbClass;
}


/** Get member variable rules */
const MemberRules& Dist_birthdeath::getMemberRules( void ) const {
    
    static MemberRules memberRules;
    static bool        rulesSet = false;
    
    if ( !rulesSet )
    {
        memberRules.push_back( new ValueRule( "lambda"   , RealPos_name     ) );
        memberRules.push_back( new ValueRule( "mu"       , RealPos_name     ) );
        memberRules.push_back( new ValueRule( "rho"      , Probability_name ) );
        memberRules.push_back( new ValueRule( "nSpecies" , Natural_name     ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/** Get random variable type */
const TypeSpec Dist_birthdeath::getVariableType( void ) const {
    
    return TypeSpec( VectorRealPos_name );
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
double Dist_birthdeath::lnPdf( const RbLanguageObject* value ) {
    
    // Get the parameters
    const VectorRealPos* times = static_cast<const VectorRealPos*>( value );
    double                 b   = static_cast<const RealPos*    >( getMemberValue( "lambda" ) )->getValue();
    double                 d   = static_cast<const RealPos*    >( getMemberValue( "mu"     ) )->getValue();
    double                 p   = static_cast<const Probability*>( getMemberValue( "rho"    ) )->getValue();
    
    // get the number of speciation events
    size_t events = times->getLength();
    
    // get the time of the orign. Note, we assume the process starts at the origin and not at the root. Further, we do not assume that the process starts with t_0 = 0.
    double now   = (*times)[events-1];
    double t_or  = now - (*times)[0] ;
    
    // calculate the probability
    double log_p = events * (2*log(b-d) - (log(p*b + (b*(1.0-p)-d)*exp((d-b)*t_or)) + log(1.0 - exp((d-b)*t_or))));
    
    // iterate over all speciation events
    for (size_t i=1; i<events; i++){
        double t = now - (*times)[i];
        log_p += (d-b)*t - 2*log(p*b + (b*(1.0-p)-d)*exp((d-b)*t));
    }
    
    return log_p;
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
double Dist_birthdeath::pdf( const RbLanguageObject* value ) {
    
    // Get the parameters
    const VectorRealPos* times = static_cast<const VectorRealPos*>( value );
    double                 b   = static_cast<const RealPos*    >( getMemberValue( "lambda" ) )->getValue();
    double                 d   = static_cast<const RealPos*    >( getMemberValue( "mu"     ) )->getValue();
    double                 p   = static_cast<const Probability*>( getMemberValue( "rho"    ) )->getValue();
    
    // get the number of speciation events
    size_t events = times->getLength();
    
    // get the time of the orign. Note, we assume the process starts at the origin and not at the root. Further, we do not assume that the process starts with t_0 = 0.
    double now   = (*times)[events-1];
    double t_or  = now - (*times)[0] ;
    
    // calculate the probability
    double log_p = events * (2*log(b-d) - (log(p*b + (b*(1.0-p)-d)*exp((d-b)*t_or)) + log(1.0 - exp((d-b)*t_or))));
    
    // iterate over all speciation events
    for (size_t i=1; i<events; i++){
        double t = now - (*times)[i];
        log_p += (d-b)*t - 2*log(p*b + (b*(1.0-p)-d)*exp((d-b)*t));
    }
    
    return exp(log_p);
}


/**
 * This function generates a vector of speciation times from the distribution
 *
 * @brief Random draw from distribution
 *
 * @return      Randomly drawn vector of speciation times
 */
VectorRealPos* Dist_birthdeath::rv( void ) {
    
    // Get the parameters
    double b        = static_cast<const RealPos*    >( getMemberValue( "lambda"   ) )->getValue();
    double d        = static_cast<const RealPos*    >( getMemberValue( "mu"       ) )->getValue();
    double p        = static_cast<const Probability*>( getMemberValue( "rho"      ) )->getValue();
    size_t nSpecies = static_cast<const Natural*    >( getMemberValue( "nSpecies" ) )->getValue();
    
    // create the vector of speciation times
    VectorRealPos *times = new VectorRealPos();
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get a new random number
    double ran = rng->uniform01();
    
    // simulate the time of the origin
	double t_or = log((-b * p - b * pow(ran,(1.0/nSpecies)) + d * pow(ran,(1.0/nSpecies)) + b * p * pow(ran,(1.0/nSpecies)))/(b * p * (-1.0 + pow(ran,(1.0/nSpecies))))) / (b - d);
    
    // tmp vector of speciation times. we need to sort this vector before we use it later
    std::vector<double> tmp_times;
    
    // simulate speciation times
    for (size_t i=0; i<nSpecies; i++) {
        ran = rng->uniform01(); 
        double t = 1.0/(p*b-(d-b*(1.0-p)))*log((p*b-(d-b*(1.0-p))* exp((-(p*b)+(d-b*(1.0-p)))*t_or) -(d-b*(1.0-p))*(1-exp((-(p*b)+(d-b*(1.0-p)))*t_or)) *ran )/(p*b-(d-b*(1.0-p))* exp((-(p*b)+(d-b*(1.0-p)))*t_or) -(p*b)*(1-exp((-(p*b)+(d-b*(1.0-p)))*t_or)) *ran )   ); 
        tmp_times.push_back(t);
    }
    
    // sort the speciation times
    sort (tmp_times.begin(), tmp_times.end());
    
    // add the times to the vector
    for (size_t i=0; i<nSpecies; i++) {
        times->push_back(tmp_times[i]);
    }
    
    
    
    return times;
}


