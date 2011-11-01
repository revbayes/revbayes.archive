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
Move::Move( RbPtr<const MemberRules> memberRules ) : ConstantMemberObject( memberRules ) {

    numAccepted = 0;
    numTried    = 0;
}

/** Copy Constructor. We use a shallow copy of the nodes */
Move::Move(const Move &m) : ConstantMemberObject(m) {
    
    numTried    = m.numTried;
    numAccepted = m.numAccepted;
    
    nodes       = m.nodes;
}

/** Destructor */
Move::~Move() {
    // release all nodes
    nodes.clear();
}


/** Map calls to member methods */
RbPtr<RbLanguageObject> Move::executeOperation(const std::string& name, const RbPtr<Environment>& args) {

    static ArgumentRules acceptArgRules;
    static ArgumentRules acceptanceRatioArgRules;    
    static ArgumentRules numAcceptedArgRules;    
    static ArgumentRules numTriedArgRules;
    static ArgumentRules proposeArgRules;
    static ArgumentRules rejectArgRules;
    static ArgumentRules resetCountersArgRules;

    if ( name == "accept" ) {

        acceptMove();
        return RbPtr<RbLanguageObject>::getNullPtr();
    }
    else if ( name == "acceptanceRatio" ) {

        return RbPtr<RbLanguageObject>( new Real( getAcceptanceRatio() ) );
    }
    else if ( name == "numAccepted" ) {

        return RbPtr<RbLanguageObject>( new Natural( numAccepted ) );
    }
    else if ( name == "numTried" ) {

        return RbPtr<RbLanguageObject>( new Natural( numTried ) );
    }
    else if ( name == "propose" ) {

        RbPtr<VectorReal> temp( new VectorReal(2) );
        RbPtr<Real> tmp( new Real(performMove( (*temp)[0] ) ) );
        
        // return the Hastings ratio
        return RbPtr<RbLanguageObject>( tmp );
    }
    else if ( name == "reject" ) {

        rejectMove();
        return RbPtr<RbLanguageObject>::getNullPtr();
    }
    else if ( name == "resetCounters" ) {

        resetCounters();
        return RbPtr<RbLanguageObject>::getNullPtr();
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
RbPtr<const MemberRules> Move::getMemberRules( void ) const {

    static RbPtr<MemberRules> memberRules( new MemberRules() );
    static bool        rulesSet = false;

    if (!rulesSet) 
		{
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "weight"  , RbPtr<RbLanguageObject>( new RealPos(1.0) ) ) ) );
        rulesSet = true;
		}

    return RbPtr<const MemberRules>( memberRules );
}


/** Get move methods */
RbPtr<const MethodTable> Move::getMethods(void) const {

    static RbPtr<MethodTable> methods( new MethodTable() );
    
    static RbPtr<ArgumentRules> acceptArgRules( new ArgumentRules() );
    static RbPtr<ArgumentRules> acceptanceRatioArgRules( new ArgumentRules() );    
    static RbPtr<ArgumentRules> numAcceptedArgRules( new ArgumentRules() );    
    static RbPtr<ArgumentRules> numRejectedArgRules( new ArgumentRules() );    
    static RbPtr<ArgumentRules> numTriedArgRules( new ArgumentRules() );
    static RbPtr<ArgumentRules> proposeArgRules( new ArgumentRules() );
    static RbPtr<ArgumentRules> rejectArgRules( new ArgumentRules() );
    static RbPtr<ArgumentRules> resetCountersArgRules( new ArgumentRules() );
    
    static bool          methodsSet = false;

    if ( methodsSet == false ) 
        {
        
        // Add functions
        methods->addFunction( "accept",          RbPtr<RbFunction>( new MemberFunction( RbVoid_name,     acceptArgRules ) ) );
        methods->addFunction( "acceptanceRatio", RbPtr<RbFunction>( new MemberFunction( Real_name,       acceptanceRatioArgRules ) ) );
        methods->addFunction( "numAccepted",     RbPtr<RbFunction>( new MemberFunction( Natural_name,    numAcceptedArgRules ) ) );
        methods->addFunction( "numRejected",     RbPtr<RbFunction>( new MemberFunction( Natural_name,    numRejectedArgRules ) ) );
        methods->addFunction( "numTried",        RbPtr<RbFunction>( new MemberFunction( Natural_name,    numTriedArgRules ) ) );
        methods->addFunction( "propose",         RbPtr<RbFunction>( new MemberFunction( VectorReal_name, proposeArgRules ) ) );
        methods->addFunction( "reject",          RbPtr<RbFunction>( new MemberFunction( RbVoid_name,     rejectArgRules ) ) );
        methods->addFunction( "resetCounters",   RbPtr<RbFunction>( new MemberFunction( RbVoid_name,     resetCountersArgRules ) ) );
        
        // Set parent table for proper inheritance
        methods->setParentTable( RbPtr<const FunctionTable>( MemberObject::getMethods() ) );
        methodsSet = true;
    }

    return RbPtr<const MethodTable>( methods );
}


/** We provide a convenience function for getting the update weight of the move
 *  from its member variable "weight". */
double Move::getUpdateWeight( void ) const {

    return static_cast<const RealPos*>( (const RbLanguageObject*)(*members)["weight"]->getValue() )->getValue();
}


/** Reset counters (numTried, numAccepted) */
void Move::resetCounters(void) {

    numAccepted = 0;
    numTried    = 0;
}

/** Set member variable. We catch here setting of the stochastic nodes to add them to our internal vector */
void Move::setMemberVariable(std::string const &name, RbPtr<Variable> var) {
    
    // test whether we want to set the variable 
    if (name == "variable") {
        // clear the nodes
        nodes.clear();
        
        // test whether we want to set multiple variable
        if (var->getValue()->isTypeSpec( TypeSpec(DagNodeContainer_name) )) {
            RbPtr<DagNodeContainer> container( dynamic_cast<DagNodeContainer*>( (RbLanguageObject*)var->getValue() ) );
            
            // add all moves
            for (size_t i=0; i<container->size(); i++) {
                RbPtr<RbObject> theElement = container->getElement(i);
                
                // check if it is a stochastic node
                if (theElement->isTypeSpec( TypeSpec(VariableNode_name) )) {
                    // cast to stochastic node
                    RbPtr<VariableNode> theNode( dynamic_cast<VariableNode*>( (RbObject*)theElement) );
                    
                    // add
                    nodes.push_back(theNode);
                }
                else {
                    throw RbException("Cannot add non variable node to a move.");
                }
            }
        }
        else if (var->getDagNode()->isType(VariableNode_name)) {
            // cast to stochastic node
            RbPtr<VariableNode> theNode( dynamic_cast<VariableNode*>( (DAGNode*)var->getDagNode() ) );
            
            // add
            nodes.push_back(theNode);
        }
        else {
            throw RbException("Cannot add non variable node to a move.");
        }
    }
    
    ConstantMemberObject::setMemberVariable(name, var);
}


void Move::replaceDagNodes(std::vector<RbPtr<VariableNode> > &n) {
    
    // release all nodes
    nodes.clear();
    
    // add all nodes
    for (size_t i=0; i<n.size(); i++) {
        RbPtr<VariableNode> theNode = n[i];
        if (theNode != NULL) {
            nodes.push_back(theNode);
        }
    }
    
}

