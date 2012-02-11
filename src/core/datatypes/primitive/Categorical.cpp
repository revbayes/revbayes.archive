/**
 * @file
 * This file contains the implementation of Categorical, which is
 * the abstract base class for categorical data types in RevBayes.
 *
 * @brief Implementation of Categorical
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */


#include "Categorical.h"
#include "Integer.h"
#include "RbException.h"
#include "RbUtil.h"
#include "Vector.h"
#include "ValueRule.h"
#include "VectorString.h"

#include <sstream>


/** Default constructor */
Categorical::Categorical( void ) : RbLanguageObject(), value( -1 ) {
}


/** Constructor from int value: check that the value is valid */
Categorical::Categorical( int x ) : RbLanguageObject(), value( x ) {
}


/** Construct from char value, taken to be a valid state symbol or NA symbol */
Categorical::Categorical( char c ) :  RbLanguageObject(), value(-1) {
}


/** Translate character symbol to state */
int Categorical::convertSymbolToState( char c ) const {

    int stateValue = 0;
    const std::string& stateLabels = getStateLabels();
//    const std::string& NALabels    = getNALabels();

    bool foundMatch = false;

    for ( size_t i = 0; i < stateLabels.size(); i++ ) {
        if ( c == stateLabels[i] ) {
            stateValue = int( i );
            foundMatch = true;
            break;
        }
    }

//    if ( foundMatch == false ) {
//        for ( size_t i = 0; i < NALabels.size(); i++ ) {
//            if ( c == NALabels[i] ) {
//                stateValue = -1 - static_cast<int>( i );
//                foundMatch = true;
//                break;
//            }
//        }
//    }

    return stateValue;
}


/** Get class vector describing type of object */
const VectorString& Categorical::getClass() const {

    static VectorString rbClass = VectorString( Categorical_name ) + RbLanguageObject::getClass();
    return rbClass;
}


const MemberRules& Categorical::getMemberRules(void) const {
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
    
    if (!rulesSet) {
        
        memberRules.push_back( new ValueRule( "state"  , Integer_name ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/** Initialize the Categorical variable. We just set the value. */
void Categorical::initialize(const Vector &attributes) {
    
    // set the state
    value = static_cast<const Integer&>( attributes[0] ).getValue();
}

/** Check if character code is valid */
bool Categorical::isValidState( int x ) const {

    if ( x >= int( getStateLabels().size() ) ) // || x < int( getNALabels().size() ) )
        return false;

    return true;
}


/** Print value for user */
void Categorical::printValue( std::ostream& o ) const {

//    if ( value < 0 )
//        o << getNALabels()[ -1 - value ];
//    else
        o << getStateLabels()[ value ];
}


/** Set value from int */
void Categorical::setValue( int x ) {

//    std::cout << "Categorical::setValue" << std::endl;
    if ( !isValidState( x ) )
        throw RbException( "Invalid state of categorical value" );

    value = x;
}

