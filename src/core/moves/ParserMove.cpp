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
#include "MethodTable.h"
#include "Natural.h"
#include "ParserMove.h"
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


const std::vector<RbPtr<DAGNode> >& ParserMove::getMoveArgumgents( void ) const {
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
void ParserMove::setMember(std::string const &name, const RbPtr<Variable> &var) {
    
    if ( attributeNames.find( name ) != attributeNames.end() ) {
        RbValue<void*> lValue = var->getValue().getLeanValue();
        move->setAttribute( name, lValue );
    }
    else {
        args.push_back( var->getDagNode() );
    }
}

/** We catch here the setting of the member variables to store our parameters. */
void ParserMove::setConstMember(std::string const &name, const RbPtr<const Variable> &var) {
    
    if ( attributeNames.find( name ) != attributeNames.end() ) {
        RbValue<void*> lValue = var->getValue().getLeanValue();
        move->setAttribute( name, lValue );
    }
    else {
        throw RbException("Cannot add non-const variable for a move.");
    }
}


