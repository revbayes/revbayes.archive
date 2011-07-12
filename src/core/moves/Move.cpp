/**
 * @file
 * This file contains the implementation of some common functions
 * in Move, which is the abstract base class for moves in mcmc
 * inference.
 *
 * @brief Partial implementation of Move
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#include "ConstantValueRule.h"
#include "DAGNode.h"
#include "MemberFunction.h"
#include "Move.h"
#include "Natural.h"
#include "RealPos.h"
#include "RbException.h"
#include "RbNames.h"
#include "ReferenceRule.h"
#include "ValueRule.h"
#include "VectorReal.h"
#include "VectorString.h"
#include "Workspace.h"

#include <iostream>
#include <string>
#include <vector>


/** Constructor for parser use */
Move::Move( const MemberRules& memberRules )
    : MemberObject( memberRules ) {

    numAccepted = 0;
    numTried    = 0;
}


/** Map calls to member methods */
DAGNode* Move::executeOperation(const std::string& name, ArgumentFrame& args) {

    static ArgumentRules acceptArgRules;
    static ArgumentRules acceptanceRatioArgRules;    
    static ArgumentRules numAcceptedArgRules;    
    static ArgumentRules numTriedArgRules;
    static ArgumentRules proposeArgRules;
    static ArgumentRules rejectArgRules;
    static ArgumentRules resetCountersArgRules;

    if ( name == "accept" ) {

        acceptMove();
        return NULL;
    }
    else if ( name == "acceptanceRatio" ) {

        return ( new Real( getAcceptanceRatio() ) )->wrapIntoVariable();
    }
    else if ( name == "numAccepted" ) {

        return ( new Natural( numAccepted ) )->wrapIntoVariable();
    }
    else if ( name == "numTried" ) {

        return ( new Natural( numTried ) )->wrapIntoVariable();
    }
    else if ( name == "propose" ) {

        VectorReal* temp = new VectorReal(2);
        performMove( (*temp)[0], (*temp)[1] );
        
        return temp->wrapIntoVariable();
    }
    else if ( name == "reject" ) {

        rejectMove();
        return NULL;
    }
    else if ( name == "resetCounters" ) {

        resetCounters();
        return NULL;
    }

    // No hit yet; we hope there is a mapped function call in the base class
    return MemberObject::executeOperation( name, args );
}


/** Calculate acceptance probability */
double Move::getAcceptanceRatio( void ) const {

    return double( numAccepted ) / double( numTried );
}


/** Get class vector describing type of object */
const VectorString& Move::getClass( void ) const { 

    static VectorString rbClass = VectorString( Move_name ) + MemberObject::getClass();
	return rbClass;
}


/** Return member rules */
const MemberRules& Move::getMemberRules( void ) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) 
		{
        memberRules.push_back(new ConstantValueRule( "weight"  , new RealPos(1.0) ) );
        rulesSet = true;
		}

    return memberRules;
}


/** Get move methods */
const MethodTable& Move::getMethods(void) const {

    static MethodTable   methods;
    
    static ArgumentRules acceptArgRules;
    static ArgumentRules acceptanceRatioArgRules;    
    static ArgumentRules numAcceptedArgRules;    
    static ArgumentRules numRejectedArgRules;    
    static ArgumentRules numTriedArgRules;
    static ArgumentRules proposeArgRules;
    static ArgumentRules rejectArgRules;
    static ArgumentRules resetCountersArgRules;
    
    static bool          methodsSet = false;

    if ( methodsSet == false ) 
        {
        // Add 'this' reference used to distinguish between different instances
        acceptArgRules.push_back         ( new ReferenceRule( "", Move_name ) );
        acceptanceRatioArgRules.push_back( new ReferenceRule( "", Move_name ) );
        numAcceptedArgRules.push_back    ( new ReferenceRule( "", Move_name ) );
        numRejectedArgRules.push_back    ( new ReferenceRule( "", Move_name ) );
        numTriedArgRules.push_back       ( new ReferenceRule( "", Move_name ) );
        proposeArgRules.push_back        ( new ReferenceRule( "", Move_name ) );
        rejectArgRules.push_back         ( new ReferenceRule( "", Move_name ) );
        resetCountersArgRules.push_back  ( new ReferenceRule( "", Move_name ) );
        
        // Add functions
        methods.addFunction( "accept",          new MemberFunction( RbVoid_name,     acceptArgRules ) );
        methods.addFunction( "acceptanceRatio", new MemberFunction( Real_name,       acceptanceRatioArgRules ) );
        methods.addFunction( "numAccepted",     new MemberFunction( Natural_name,    numAcceptedArgRules ) );
        methods.addFunction( "numRejected",     new MemberFunction( Natural_name,    numRejectedArgRules ) );
        methods.addFunction( "numTried",        new MemberFunction( Natural_name,    numTriedArgRules ) );
        methods.addFunction( "propose",         new MemberFunction( VectorReal_name, proposeArgRules ) );
        methods.addFunction( "reject",          new MemberFunction( RbVoid_name,     rejectArgRules ) );
        methods.addFunction( "resetCounters",   new MemberFunction( RbVoid_name,     resetCountersArgRules ) );
        
        // Set parent table for proper inheritance
        methods.setParentTable( const_cast<MethodTable*>( &MemberObject::getMethods() ) );
        methodsSet = true;
    }

    return methods;
}


/** We provide a convenience function for getting the update weight of the move
 *  from its member variable "weight". */
double Move::getUpdateWeight( void ) const {

    return static_cast<const RealPos*>( members["weight"].getValue() )->getValue();
}


/** Reset counters (numTried, numAccepted) */
void Move::resetCounters(void) {

    numAccepted = 0;
    numTried    = 0;
}

