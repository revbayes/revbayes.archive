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
#include "VectorInteger.h"
#include "VectorReal.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cmath>
#include <sstream>
#include <vector>


/** Default constructor for parser use */
Dist_multinomial::Dist_multinomial( void ) : DistributionDiscrete( getMemberRules() ), probabilities( NULL ) {
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
    
    return static_cast<Simplex&>( probabilities->getValue() );
}


/** Get the state vector for this distribution */
const std::vector<RbLanguageObject*>& Dist_multinomial::getStateVector( void ) const {
    throw RbException("getStateVector not implemented for multinomial distribution.");
}


/** Get random variable type */
const TypeSpec& Dist_multinomial::getVariableType( void ) const {

    static TypeSpec varTypeSpec = VectorNatural::getClassTypeSpec();
    
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
double Dist_multinomial::lnPdf( const RbLanguageObject& value ) const {

	// Get the value and the parameters of the Dirichlet
    std::vector<double>       p = static_cast<const Simplex&      >( probabilities->getValue() ).getValue();
    std::vector<unsigned int> x = static_cast<const VectorNatural&>( value ).getValue();

	// Check that the vectors are both the same size
	if ( p.size() != x.size() )
		throw RbException( "Inconsistent size of vectors when calculating Multinomial log probability density" );

	return RbStatistics::Multinomial::lnPdf( p, x );
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
double Dist_multinomial::pdf( const RbLanguageObject& value ) const {

	// Get the value and the parameters of the Dirichlet
    std::vector<double>       p = static_cast<const Simplex&      >( probabilities->getValue() ).getValue();
    std::vector<unsigned int> x = static_cast<const VectorNatural&>( value ).getValue();

	// check that the vectors are both the same size
	if ( p.size() != x.size() )
		throw RbException( "Inconsistent size of vectors when calculating Multinomial log probability density" );

	return RbStatistics::Multinomial::pdf( p, x );
}


/**
 * This function generates a multinomial-distributed
 * random variable.
 *
 * @brief Random draw from multinomial distribution
 *
 * @return      Random draw from multinomial distribution
 */
const RbLanguageObject& Dist_multinomial::rv( void ) {

    std::vector<double> p      = static_cast<Simplex&>( probabilities->getValue() ).getValue();
    RandomNumberGenerator* rng = GLOBAL_RNG;
	std::vector<int> r( p.size() );

	r = RbStatistics::Multinomial::rv( p, *rng );
    randomVariable.setValue( r );
    
    return randomVariable;
}


/** We catch here the setting of the member variables to store our parameters. */
void Dist_multinomial::setMemberVariable(std::string const &name, Variable *var) {
    
    if ( name == "probabilities" ) {
        probabilities = var;
    }
    else {
        DistributionDiscrete::setMemberVariable(name, var);
    }
}

