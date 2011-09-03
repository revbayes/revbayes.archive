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
Move::Move( const MemberRules& memberRules ) : ConstantMemberObject( memberRules ) {

    numAccepted = 0;
    numTried    = 0;
}

/** Copy Constructor. We use a shallow copy of the nodes */
Move::Move(const Move &m) : ConstantMemberObject(m) {
    
    numTried    = m.numTried;
    numAccepted = m.numAccepted;
    
    nodes       = m.nodes;
    // retain all nodes
    for (size_t i=0; i<nodes.size(); i++) {
        nodes[i]->retain();
    }
}

/** Destructor */
Move::~Move() {
    // release all nodes
    for (size_t i=0; i<nodes.size(); i++) {
        VariableNode *theNode = nodes[i];
        if (theNode != NULL) {
            theNode->release();
            if (theNode->isUnreferenced())
                delete theNode;
        }
    }
    nodes.clear();
}


/** Map calls to member methods */
RbLanguageObject* Move::executeOperation(const std::string& name, Environment& args) {

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

        return ( new Real( getAcceptanceRatio() ) );
    }
    else if ( name == "numAccepted" ) {

        return ( new Natural( numAccepted ) );
    }
    else if ( name == "numTried" ) {

        return ( new Natural( numTried ) );
    }
    else if ( name == "propose" ) {

        VectorReal* temp = new VectorReal(2);
        Real *tmp = new Real(performMove( (*temp)[0] ));
        
        // return the Hastings ratio
        return tmp;
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
        memberRules.push_back(new ValueRule( "weight"  , new RealPos(1.0) ) );
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

/** Set member variable. We catch here setting of the stochastic nodes to add them to our internal vector */
void Move::setMemberVariable(std::string const &name, Variable *var) {
    
    // test whether we want to set the variable 
    if (name == "variable") {
        // clear the nodes
        for (size_t i=0; i<nodes.size(); i++) {
            VariableNode *theNode = nodes[i];
            if (theNode != NULL) {
                theNode->release();
                if (theNode->isUnreferenced()) {
                    delete theNode;
                }
            }
        }
        nodes.clear();
        
        // test whether we want to set multiple variable
        if (var->getValue()->isTypeSpec( TypeSpec(DagNodeContainer_name) )) {
            DagNodeContainer *container = dynamic_cast<DagNodeContainer*>(var->getDagNodePtr()->getValuePtr());
            
            // add all moves
            for (size_t i=0; i<container->size(); i++) {
                RbObject *theElement = container->getElement(i);
                
                // check if it is a stochastic node
                if (theElement->isTypeSpec( TypeSpec(VariableNode_name) )) {
                    // cast to stochastic node
                    VariableNode *theNode = dynamic_cast<VariableNode*>(theElement);
                    
                    // add
                    nodes.push_back(theNode);
                    
                    //retain
                    theNode->retain();
                }
                else {
                    throw RbException("Cannot add non variable node to a move.");
                }
            }
        }
        else if (var->getDagNode()->isType(VariableNode_name)) {
            // cast to stochastic node
            VariableNode *theNode = dynamic_cast<VariableNode*>(var->getDagNodePtr());
            
            // add
            nodes.push_back(theNode);
            
            //retain
            theNode->retain();
        }
        else {
            throw RbException("Cannot add non variable node to a move.");
        }
    }
    
    ConstantMemberObject::setMemberVariable(name, var);
}


void Move::replaceDagNodes(std::vector<VariableNode *> &n) {
    
    // release all nodes
    for (size_t i=0; i<nodes.size(); i++) {
        VariableNode *theNode = nodes[i];
        if (theNode != NULL) {
            theNode->release();
            if (theNode->isUnreferenced())
                delete theNode;
        }
    }
    nodes.clear();
    
    // add all nodes
    for (size_t i=0; i<n.size(); i++) {
        VariableNode *theNode = n[i];
        if (theNode != NULL) {
            theNode->retain();
            nodes.push_back(theNode);
        }
    }
    
}

