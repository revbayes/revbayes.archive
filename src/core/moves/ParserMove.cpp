/**
 * @file
 * This file contains the implementation of some common functions
 * in Move, which is the abstract base class for moves in mcmc
 * inference.
 *
 * @brief Partial implementation of Move
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-16 17:27:08 -0700 (Mon, 16 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id: Move.cpp 1378 2012-04-17 00:27:08Z hoehna $
 */

#include "DAGNode.h"
#include "DagNodeContainer.h"
#include "MemberFunction.h"
#include "MethodTable.h"
#include "Natural.h"
#include "ParserMove.h"
#include "RealPos.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "VariableNode.h"
#include "Workspace.h"

#include <iostream>
#include <string>
#include <vector>


/** Constructor */
ParserMove::ParserMove( InferenceMove* m, const std::string &n, const MemberRules& mr, const std::set<std::string> &attrNames ) : MemberObject( mr ), move( m ), memberRules( mr ), typeSpec( getClassName() + " (" + n + ")", new TypeSpec( MemberObject::getClassTypeSpec() ) ), attributeNames( attrNames ) {

}

/** Copy Constructor. 
 * We don't copy the nodes and we don't own them.
 */
ParserMove::ParserMove(const ParserMove &m) : MemberObject(m), move( m.move->clone() ), memberRules( m.memberRules ), typeSpec( m.typeSpec ), args( m.args ), attributeNames( m.attributeNames ) {
    
}

/** Destructor */
ParserMove::~ParserMove() {
    delete move;
}


ParserMove* ParserMove::clone( void ) const {
    return new ParserMove(*this);
}


///** Map calls to member methods */
//const RbLanguageObject& ParserMove::executeOperationSimple(const std::string& name, const std::vector<Argument>& args) {
//    
//    if ( name == "accept" ) {
//        
//        acceptMove();
//        return RbNullObject::getInstance();
//    }
//    else if ( name == "acceptanceRatio" ) {
//        
//        acceptanceR.setValue( getAcceptanceRatio() );
//        return acceptanceR;
//    }
//    else if ( name == "numAccepted" ) {
//        
//        return numAccepted;
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
//    
//    // No hit yet; we hope there is a mapped function call in the base class
//    return MemberObject::executeOperationSimple( name, args );
//}



/** Get class name of object */
const std::string& ParserMove::getClassName(void) { 
    
    static std::string rbClassName = "Move";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& ParserMove::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


const InferenceMove* ParserMove::getLeanMove( void ) const {
    return move;
}


/** Return member rules */
const MemberRules& ParserMove::getMemberRules( void ) const {
    
    return memberRules;
}


///** Get move methods */
//const MethodTable& ParserMove::getMethods(void) const {
//    
//    static MethodTable methods = MethodTable();
//    
//    static ArgumentRules* acceptArgRules            = new ArgumentRules();
//    static ArgumentRules* acceptanceRatioArgRules   = new ArgumentRules();    
//    static ArgumentRules* numAcceptedArgRules       = new ArgumentRules();    
//    static ArgumentRules* numRejectedArgRules       = new ArgumentRules();    
//    static ArgumentRules* numTriedArgRules          = new ArgumentRules();
//    static ArgumentRules* proposeArgRules           = new ArgumentRules();
//    static ArgumentRules* rejectArgRules            = new ArgumentRules();
//    static ArgumentRules* resetCountersArgRules     = new ArgumentRules();
//    
//    static bool          methodsSet = false;
//    
//    if ( methodsSet == false ) 
//    {
//        
//        // Add functions
//        methods.addFunction( "accept",          new MemberFunction( RbVoid_name,     acceptArgRules            ) );
//        methods.addFunction( "acceptanceRatio", new MemberFunction( Real::getClassTypeSpec(),       acceptanceRatioArgRules   ) );
//        methods.addFunction( "numAccepted",     new MemberFunction( Natural::getClassTypeSpec(),    numAcceptedArgRules       ) );
//        methods.addFunction( "numRejected",     new MemberFunction( Natural::getClassTypeSpec(),    numRejectedArgRules       ) );
//        methods.addFunction( "numTried",        new MemberFunction( Natural::getClassTypeSpec(),    numTriedArgRules          ) );
//        //        methods.addFunction( "propose",         new MemberFunction( RbVector<Real>::getClassTypeSpec(), proposeArgRules           ) );
//        methods.addFunction( "reject",          new MemberFunction( RbVoid_name,     rejectArgRules            ) );
//        methods.addFunction( "resetCounters",   new MemberFunction( RbVoid_name,     resetCountersArgRules     ) );
//        
//        // Set parent table for proper inheritance
//        methods.setParentTable( &MemberObject::getMethods() );
//        methodsSet = true;
//    }
//    
//    return methods;
//}


const std::vector<RbConstDagNodePtr>& ParserMove::getMoveArgumgents( void ) const {
    return args;
}


const TypeSpec& ParserMove::getTypeSpec( void ) const {
    return typeSpec;
}


void ParserMove::printValue(std::ostream &o) const {
    // \TODO: Need some more meaningful output
    o << "Move";
}


/** We catch here the setting of the member variables to store our parameters. */
void ParserMove::setMemberVariable(std::string const &name, const Variable* var) {
    
    if ( attributeNames.find( name ) != attributeNames.end() ) {
        RbValue<void*> lValue;
        lValue.value = var->getValue().getLeanValue( lValue.lengths );
        move->setAttribute( name, lValue );
    }
    else {
        args.push_back( var->getDagNode() );
    }
}


