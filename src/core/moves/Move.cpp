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

#include "ConstArgumentRule.h"
#include "DAGNode.h"
#include "DagNodeContainer.h"
#include "MethodTable.h"
#include "Move.h"
#include "Natural.h"
#include "Probability.h"
#include "RealPos.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "SimpleMemberFunction.h"
#include "StochasticNode.h"
#include "VariableNode.h"
#include "Workspace.h"

#include <iostream>
#include <string>
#include <vector>


/** Constructor */
Move::Move( const MemberRules& memberRules ) : MemberObject( memberRules ) {

}


/* Add a DAG node */
void Move::addDagNode(StochasticNode *d) {
    nodes.push_back( d );
}


/** Map calls to member methods */
RbPtr<RbLanguageObject> Move::executeSimpleMethod(const std::string& name, const std::vector<const RbObject *>& args) {

    if ( name == "accept" ) {

        acceptMove();
        return NULL;
    }
//    else if ( name == "acceptanceRatio" ) {

//        return RbPtr<RbLanguageObject>( new Probability( getAcceptanceRatio() ) );
//    }
//    else if ( name == "numAccepted" ) {
//
//        return RbPtr<RbLanguageObject>( new Natural( numAccepted ) ) );
//    }
//    else if ( name == "numTried" ) {
//
//        return numTried;
//    }
//    else if ( name == "propose" ) {
//
//        double probRatio;
//        Real* tmp = new Real(performMove( probRatio ) );
//        
//        // return the acceptance ratio
//        return *tmp;
//    }
//    else if ( name == "reject" ) {
//
//        rejectMove();
//        return RbNullObject::getInstance();
//    }
//    else if ( name == "resetCounters" ) {
//
//        resetCounters();
//        return RbNullObject::getInstance();
//    }

    // No hit yet; we hope there is a mapped function call in the base class
    return MemberObject::executeSimpleMethod( name, args );
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
        memberRules.push_back( new ConstArgumentRule( "weight"  , new RealPos(1.0) ) );
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
        methods.addFunction( "accept",          new SimpleMemberFunction( RbVoid_name,     acceptArgRules            ) );
        methods.addFunction( "acceptanceRatio", new SimpleMemberFunction( Real::getClassTypeSpec(),       acceptanceRatioArgRules   ) );
        methods.addFunction( "numAccepted",     new SimpleMemberFunction( Natural::getClassTypeSpec(),    numAcceptedArgRules       ) );
        methods.addFunction( "numRejected",     new SimpleMemberFunction( Natural::getClassTypeSpec(),    numRejectedArgRules       ) );
        methods.addFunction( "numTried",        new SimpleMemberFunction( Natural::getClassTypeSpec(),    numTriedArgRules          ) );
//        methods.addFunction( "propose",         new MemberFunction( RbVector<Real>::getClassTypeSpec(), proposeArgRules           ) );
        methods.addFunction( "reject",          new SimpleMemberFunction( RbVoid_name,     rejectArgRules            ) );
        methods.addFunction( "resetCounters",   new SimpleMemberFunction( RbVoid_name,     resetCountersArgRules     ) );
        
        // Set parent table for proper inheritance
        methods.setParentTable( &MemberObject::getMethods() );
        methodsSet = true;
    }

    return methods;
}


void Move::printValue(std::ostream &o) const {
   // \TODO: Need some more meaningful output
    o << "Move";
}


