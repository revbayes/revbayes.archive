/**
 * @file
 * This file contains the implementation of Dist_cat, which is used to hold
 * parameters and functions related to a distribution on a categorical
 * variable (a factor).
 *
 * @brief Implementation of Dist_cat
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

#include "DAGNode.h"
#include "Dist_cat.h"
#include "Move_msimplex.h"
#include "RealPos.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "Real.h"
#include "Set.h"
#include "Simplex.h"
#include "ValueRule.h"
#include "Workspace.h"

#include <cmath>
#include <sstream>
#include <vector>


/** Default constructor for parser use */
Dist_cat::Dist_cat( void ) : DistributionDiscrete( getMemberRules() ), probabilities( NULL ), states( NULL ) {
}


/** Clone this object */
Dist_cat* Dist_cat::clone( void ) const {

    return new Dist_cat( *this );
}


/** Get class name of object */
const std::string& Dist_cat::getClassName(void) { 
    
    static std::string rbClassName = "Categorical distribution";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Dist_cat::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( DistributionDiscrete::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Dist_cat::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

/** Get member variable rules */
const MemberRules& Dist_cat::getMemberRules( void ) const {

    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;

    if ( !rulesSet )
		{
            memberRules.push_back( new ValueRule( "probabilities", Simplex::getClassTypeSpec() ) );
            memberRules.push_back( new ValueRule( "states",        Set<RbLanguageObject>::getClassTypeSpec() ) );

        rulesSet = true;
		}

    return memberRules;
}


/** Get the number of states in the distribution */
size_t Dist_cat::getNumberOfStates( void ) const {

    return static_cast<const Simplex&>( probabilities->getValue() ).size();
}


/** Get the probability mass vector */
const Simplex& Dist_cat::getProbabilityMassVector( void ) {

    return static_cast<Simplex&>( probabilities->getValue() );
}


/** Get the state vector for this distribution */
const std::vector<RbLanguageObject*>& Dist_cat::getStateVector( void ) const {
    return stateVector;
}


/** Get random variable type */
const TypeSpec& Dist_cat::getVariableType( void ) const {
    
    if (states == NULL || RbNullObject::getInstance() == states->getValue() ) 
        return CharacterStateDiscrete::getClassTypeSpec();

    return states->getValue().getTypeSpec();
}


/**
 * This function calculates the natural log of the
 * probability of a categorical random variable.
 *
 * @brief Natural log of categorical probability
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
double Dist_cat::lnPdf( const RbLanguageObject& value ) const {

	// Get the value and the parameters of the categorical distribution
    std::vector<double>          m = static_cast<const Simplex&               >( probabilities->getValue() ).getValue();
    const Set<RbLanguageObject>& x = static_cast<const Set<RbLanguageObject>& >( states->getValue()        );

    int index = x.findIndex( value );
    if ( index < 0 )
        return 0.0;
    else
        return std::log( m[index] );
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
double Dist_cat::pdf( const RbLanguageObject& value ) const {

	// Get the value and the parameter of the categorical distribution
          std::vector<double>       m = static_cast<const Simplex&               >( probabilities->getValue() ).getValue();
    const Set<RbLanguageObject>&    s = static_cast<const Set<RbLanguageObject>& >( states->getValue()        );
    
    int index = s.findIndex( value );
    if ( index < 0 )
        return 0.0;
    else
        return m[index];
}


/**
 * This function generates a categorical
 * random variable.
 *
 * @brief Random draw from categorical distribution
 *
 * @return      Random draw from categorical distribution
 */
const RbLanguageObject& Dist_cat::rv( void ) {
    
	// Get the value and the parameter of the categorical distribution
    std::vector<double>       m = static_cast<const Simplex&               >( probabilities->getValue() ).getValue();
    const Set<RbLanguageObject>&    s = static_cast<const Set<RbLanguageObject>& >( states->getValue()        );
    
    RandomNumberGenerator* rng = GLOBAL_RNG;

    // Get copy of reference object
    delete randomVariable;

    // Draw a random value
    double r   = rng->uniform01();
    double sum = 0.0;
    size_t i;
    for ( i=0; i<m.size(); i++ ) {
        sum += m[i];
        if ( sum > r )
            break;
    }

    // get the random variable by accessing our state vector, selecting the i-th element and cloning it
    randomVariable = static_cast<RbLanguageObject*>( s.getElement( i ).clone() );


    // Return draw
    return *randomVariable;
}

