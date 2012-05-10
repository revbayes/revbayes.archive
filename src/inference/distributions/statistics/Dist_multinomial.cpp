/**
 * @file
 * This file contains the implementation of Dist_multinomial, which is used to hold
 * parameters and functions related to a multinomial distribution.
 *
 * @brief Implementation of Dist_multinomial
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "Dist_multinomial.h"
#include "DistributionMultinomial.h"
#include "Move_mmultinomial.h"
#include "RealPos.h"
#include "RandomNumberGenerator.h"
#include "Real.h"
#include "RbException.h"
#include "RbUtil.h"
#include "Simplex.h"
#include "ValueRule.h"
#include "Workspace.h"

#include <cmath>
#include <sstream>
#include <vector>


/** Default constructor for parser use */
Dist_multinomial::Dist_multinomial( void ) : DistributionDiscrete(  ) {
}


/** Clone this object */
Dist_multinomial* Dist_multinomial::clone( void ) const {

    return new Dist_multinomial( *this );
}


/** Get class name of object */
const std::string& Dist_multinomial::getClassName(void) { 
    
    static std::string rbClassName = "Multinomial distribution";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Dist_multinomial::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( DistributionDiscrete::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Dist_multinomial::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get member variable rules */
const MemberRules& Dist_multinomial::getMemberRules( void ) const {

    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;

    if ( !rulesSet )
		{
            memberRules.push_back( new ValueRule( "probabilities", Simplex::getClassTypeSpec() ) );

        rulesSet = true;
		}

    return memberRules;
}


/** Get the number of states in the distribution */
size_t Dist_multinomial::getNumberOfStates( void ) const {
    
    return static_cast<const Simplex&>( probabilities->getValue() ).size();
}


/** Get the probability mass vector */
const Simplex& Dist_multinomial::getProbabilityMassVector( void ) {
    
    return static_cast<const Simplex&>( probabilities->getValue() );
}


/** Get the state vector for this distribution */
const std::vector<RbLanguageObject*>& Dist_multinomial::getStateVector( void ) const {
    throw RbException("getStateVector not implemented for multinomial distribution.");
}


/** Get random variable type */
const TypeSpec& Dist_multinomial::getVariableType( void ) const {

    static TypeSpec varTypeSpec = RbVector<Natural>::getClassTypeSpec();
    
    return varTypeSpec;
}


/**
 * This function calculates the natural log of the probability
 * density for a multinomially-distributed random variable.
 *
 * @brief Natural log of multinomial probability density
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
double Dist_multinomial::lnPdfSingleValue( std::vector<size_t> &offset ) const {
    
    return RbStatistics::Multinomial::lnPdf(probabilities.value[offset[0]], randomVariable.value[offset[1]]);
}


/**
 * This function calculates the probability
 * for a multinomially-distributed random variable.
 *
 * @brief Multinomial probability
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_multinomial::pdfSingleValue( std::vector<size_t> &offset ) const {
    
    return RbStatistics::Multinomial::pdf(probabilities.value[offset[0]], randomVariable.value[offset[1]]);
}


/**
 * This function generates a multinomial-distributed
 * random variable.
 *
 * @brief Random draw from multinomial distribution
 *
 * @return      Random draw from multinomial distribution
 */
void Dist_multinomial::rvSingleValue( std::vector<size_t> &offset ) {
    
    RandomNumberGenerator* rng = GLOBAL_RNG;
    randomVariable.value[offset[1]] = RbStatistics::Multinomial::rv(probabilities.value[offset[0]], *rng);
    
}


/** We catch here the setting of the member variables to store our parameters. */
void Dist_multinomial::setInternalParameters(const std::vector<RbValue<void *> > &p) {
    
    probabilities.value     = static_cast<double*>( p[0].value );
    probabilities.lengths   = p[0].lengths;
    
    randomVariable.value    = static_cast<int*>( p[1].value );
    randomVariable.lengths  = p[1].lengths;
    
}

