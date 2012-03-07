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

#include "DAGNode.h"
#include "DagNodeContainer.h"
#include "MemberFunction.h"
#include "Move.h"
#include "Natural.h"
#include "RealPos.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "VariableNode.h"
#include "VectorReal.h"
#include "VectorString.h"
#include "Workspace.h"

#include <iostream>
#include <string>
#include <vector>


/** Constructor */
Move::Move( const MemberRules& memberRules ) : MemberObject( memberRules ), weight( NULL ) {

    numAccepted = 0;
    numTried    = 0;
}

/** Copy Constructor. We use a shallow copy of the nodes */
Move::Move(const Move &m) : MemberObject(m), weight( m.weight ) {
    
    numTried    = m.numTried;
    numAccepted = m.numAccepted;
    
}

/** Destructor */
Move::~Move() {
    
}


/** Map calls to member methods */
const RbLanguageObject& Move::executeOperationSimple(const std::string& name, const std::vector<Argument>& args) {

    if ( name == "accept" ) {

        acceptMove();
        return RbNullObject::getInstance();
    }
    else if ( name == "acceptanceRatio" ) {

        acceptanceR.setValue( getAcceptanceRatio() );
        return acceptanceR;
    }
    else if ( name == "numAccepted" ) {

        return numAccepted;
    }
    else if ( name == "numTried" ) {

        return numTried;
    }
    else if ( name == "propose" ) {

        double probRatio;
        Real* tmp = new Real(performMove( probRatio ) );
        
        // return the acceptance ratio
        return *tmp;
    }
    else if ( name == "reject" ) {

        rejectMove();
        return RbNullObject::getInstance();
    }
    else if ( name == "resetCounters" ) {

        resetCounters();
        return RbNullObject::getInstance();
    }

    // No hit yet; we hope there is a mapped function call in the base class
    return MemberObject::executeOperationSimple( name, args );
}


/** Calculate acceptance probability */
double Move::getAcceptanceRatio( void ) const {

    return double( numAccepted.getValue() ) / double( numTried.getValue() );
}


/** Get class name of object */
const std::string& Move::getClassName(void) { 
    
    static std::string rbClassName = "Move";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Return member rules */
const MemberRules& Move::getMemberRules( void ) const {

    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;

    if (!rulesSet) {
        memberRules.push_back( new ValueRule( "weight"  , new RealPos(1.0) ) );
        rulesSet = true;
    }

    return memberRules;
}


/** Get move methods */
const MethodTable& Move::getMethods(void) const {

    static MethodTable methods = MethodTable();
    
    static ArgumentRules* acceptArgRules            = new ArgumentRules();
    static ArgumentRules* acceptanceRatioArgRules   = new ArgumentRules();    
    static ArgumentRules* numAcceptedArgRules       = new ArgumentRules();    
    static ArgumentRules* numRejectedArgRules       = new ArgumentRules();    
    static ArgumentRules* numTriedArgRules          = new ArgumentRules();
    static ArgumentRules* proposeArgRules           = new ArgumentRules();
    static ArgumentRules* rejectArgRules            = new ArgumentRules();
    static ArgumentRules* resetCountersArgRules     = new ArgumentRules();
    
    static bool          methodsSet = false;

    if ( methodsSet == false ) 
        {
        
        // Add functions
        methods.addFunction( "accept",          new MemberFunction( RbVoid_name,     acceptArgRules            ) );
        methods.addFunction( "acceptanceRatio", new MemberFunction( Real::getClassTypeSpec(),       acceptanceRatioArgRules   ) );
        methods.addFunction( "numAccepted",     new MemberFunction( Natural::getClassTypeSpec(),    numAcceptedArgRules       ) );
        methods.addFunction( "numRejected",     new MemberFunction( Natural::getClassTypeSpec(),    numRejectedArgRules       ) );
        methods.addFunction( "numTried",        new MemberFunction( Natural::getClassTypeSpec(),    numTriedArgRules          ) );
        methods.addFunction( "propose",         new MemberFunction( VectorReal::getClassTypeSpec(), proposeArgRules           ) );
        methods.addFunction( "reject",          new MemberFunction( RbVoid_name,     rejectArgRules            ) );
        methods.addFunction( "resetCounters",   new MemberFunction( RbVoid_name,     resetCountersArgRules     ) );
        
        // Set parent table for proper inheritance
        methods.setParentTable( &MemberObject::getMethods() );
        methodsSet = true;
    }

    return methods;
}


/** We provide a convenience function for getting the update weight of the move
 *  from its member variable "weight". */
double Move::getUpdateWeight( void ) const {

    return static_cast<const RealPos&>( weight->getValue() ).getValue();
}


void Move::printValue(std::ostream &o) const {
   // TODO: Need some more meaningful output
    o << "Move";
}


/** Reset counters (numTried, numAccepted) */
void Move::resetCounters(void) {

    numAccepted = 0;
    numTried    = 0;
}

/** We catch here the setting of the member variables to store our parameters. */
void Move::setMemberVariable(std::string const &name, Variable* var) {
    
    // test whether we want to set the variable 
    if (name == "weight") {
        weight = var;
    } 
    else {
        MemberObject::setMemberVariable(name, var);
    }
}


