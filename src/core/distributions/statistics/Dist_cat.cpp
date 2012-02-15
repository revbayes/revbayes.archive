/**
 * @file
 * This file contains the implementation of Dist_cat, which is used to hold
 * parameters and functions related to a distribution on a categorical
 * variable (a factor).
 *
 * @brief Implementation of Dist_cat
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-14 12:43:32 +0100 (Mån, 14 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z ronquist $
 */

#include "DAGNode.h"
#include "Dist_cat.h"
#include "Move_msimplex.h"
#include "RealPos.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbUtil.h"
#include "Real.h"
#include "Simplex.h"
#include "ValueRule.h"
#include "VectorRealPos.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cmath>
#include <sstream>
#include <vector>


// Definition of the static type spec member
const TypeSpec Dist_cat::typeSpec(Dist_cat_name);
const TypeSpec Dist_cat::varTypeSpec(Categorical_name);

/** Default constructor for parser use */
Dist_cat::Dist_cat( void ) : DistributionDiscrete( getMemberRules() ) {
}


/** Clone this object */
Dist_cat* Dist_cat::clone( void ) const {

    return new Dist_cat( *this );
}


/** Get class vector showing type of object */
const VectorString& Dist_cat::getClass( void ) const {

    static VectorString rbClass = VectorString( Dist_cat_name ) + DistributionDiscrete::getClass();
    return rbClass;
}

/** Get member variable rules */
const MemberRules& Dist_cat::getMemberRules( void ) const {

    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;

    if ( !rulesSet )
		{
        memberRules.push_back( new ValueRule( "m"    , Simplex_name ) );
        memberRules.push_back( new ValueRule( "dummy", Categorical_name) );

        rulesSet = true;
		}

    return memberRules;
}


/** Get the number of states in the distribution */
size_t Dist_cat::getNumberOfStates( void ) const {

    return static_cast<const Simplex&>( getMemberValue("m") ).size();
}


/** Get the probability mass vector */
const Simplex& Dist_cat::getProbabilityMassVector( void ) {

    return static_cast<Simplex&>( getMemberValue("m") );
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Dist_cat::getTypeSpec(void) const {
    return typeSpec;
}


/** Get random variable type */
const TypeSpec& Dist_cat::getVariableType( void ) const {
    
    if (getMemberDagNode( "dummy" ) == NULL) 
        return varTypeSpec;

    return getMemberDagNode( "dummy" )->getValueTypeSpec();
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
    std::vector<double> m = static_cast<const Simplex&    >( getMemberValue("m") ).getValue();
    int                 x = static_cast<const Categorical&>( value ).getValue();

    if ( x < 0 )
        return 0.0;
    else
        return std::log( m[x] );
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
    std::vector<double> m = static_cast<const Simplex&    >( getMemberValue("m") ).getValue();
    int                 x = static_cast<const Categorical&>( value ).getValue();

	if ( x < 0 )
        return 1.0;
    else
        return m[x];
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

	// Get the parameter of the categorical distribution and the rng
    std::vector<double>    m   = static_cast<Simplex&>( getMemberValue( "m" ) ).getValue();
    RandomNumberGenerator* rng = GLOBAL_RNG;

    // Get copy of reference object
    if (randomVariable != NULL) {
        delete randomVariable;
    }
    randomVariable = static_cast<Categorical*>( getMemberValue( "dummy" ).clone() );

    // Draw a random value
    double r   = rng->uniform01();
    double sum = 0.0;
    size_t i;
    for ( i=0; i<m.size(); i++ ) {
        sum += m[i];
        if ( sum > r )
            break;
    }

    // Set draw to this value
    randomVariable->setValue( int( i ) );

    // Return draw
    return *randomVariable;
}

