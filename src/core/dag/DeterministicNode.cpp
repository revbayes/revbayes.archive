/**
 * @file
 * This file contains the implementation of some function in DeterministicNode,
 * which is an abstract base class for DAG nodes associated with expressions
 * (equations) determining their value.
 *
 * @brief Partial implementation of DeterministicNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-30 00:19:25 +0100 (Ons, 30 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: DeterministicNode.cpp 216 2009-12-29 23:19:25Z ronquist $
 */

#include "ArgumentRule.h"
#include "Boolean.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbNames.h"
#include "UserInterface.h"
#include "VectorString.h"
#include "Workspace.h"

#include <algorithm>
#include <cassert>


/** Constructor of empty deterministic node */
DeterministicNode::DeterministicNode( const std::string& valType ) : VariableNode( valType ), changed( false ) {
}

/** Constructor of empty deterministic node */
DeterministicNode::DeterministicNode( RbFunction* func ) : VariableNode(func->getReturnType()), changed( false ) {
    
    /* Check for cycles */
    const Environment& arguments = func->getArguments();
    std::list<DAGNode*> done;
    for ( size_t i = 0; i < arguments.size(); i++ ) {
        const std::string &name = arguments.getName(i);
        if ( arguments[name].getDagNode() != NULL && arguments[name].getDagNode()->isParentInDAG( this, done ) )
            throw RbException( "Invalid assignment: cycles in the DAG" );
    }
    
    // only add myself as a child of the arguments if the functions says so
    if (func->addAsChildOfArguments()) {
        /* Set parents and add this node as a child node of these */
        for ( size_t i = 0; i < arguments.size(); i++ ) {
            const std::string &name = arguments.getName(i);
        
            DAGNode* theArgument = arguments[name].getDagNodePtr();
            addParentNode( theArgument );
            theArgument->addChildNode( this );
        }
    }
    
    /* Set the function */
    function = func;
    function->retain();
    
    /* Set value and stored value */
    RbLanguageObject* retVal = function->execute();
    
    value           = retVal;
    if (value != NULL)
        value->retain();
    storedValue     = NULL;
}


/** Copy constructor */
DeterministicNode::DeterministicNode( const DeterministicNode& x ) : VariableNode( x ) {
    
    function         = x.function->clone();
    function->retain();
    touched          = x.touched;
    changed          = x.changed;
    value            = x.value->clone();
    value->retain();
    if ( x.storedValue != NULL ) {
        storedValue  = x.storedValue->clone();
        storedValue->retain();
    }
    else
        storedValue  = NULL;
    
    /* Set parents and add this node as a child node of these */
    const Environment& args = function->getArguments();
    for ( size_t i = 0; i < args.size(); i++ ) {
        const std::string &name = args.getName(i);
        
        DAGNode* theArgument = args[name].getDagNodePtr();
        addParentNode( theArgument );
        theArgument->addChildNode( this );
    }
}


/** Destructor */
DeterministicNode::~DeterministicNode( void ) {
    
    /* Remove parents first so that VariableNode destructor does not get in the way */
    for ( std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); i++ ) {
        (*i)->removeChildNode(this);
        removeParentNode(*i);
        if ((*i)->isUnreferenced()) {
            delete *i;
        }
    }
    parents.clear();
    
    /* Deleting the function will remove all the parent nodes when appropriate */
    if ( function != NULL) {
        function->release();
        if (function->isUnreferenced()) {
            delete function;
        }
    }

}

DeterministicNode* DeterministicNode::clone() const {
    return new DeterministicNode(*this);
}


/** Clone the entire graph: clone children, swap parents */
DeterministicNode* DeterministicNode::cloneDAG( std::map<const DAGNode*, DAGNode*>& newNodes ) const {
    
    if ( newNodes.find( this ) != newNodes.end() )
        return static_cast<DeterministicNode*>( newNodes[ this ] );
    
    /* Get pristine copy */
    DeterministicNode* copy = new DeterministicNode( getValueType() );
    newNodes[ this ] = copy;
    
    /* Set the name so that the new node remains identifiable */
    copy->setName(name);
    
    /* Set the copy member variables */
    copy->function = function->clone();
    copy->touched  = touched;
    copy->changed  = changed;
    if (value != NULL)
        copy->value    = value->clone();
    if (storedValue == NULL)
        copy->storedValue = NULL;
    else
        copy->storedValue = storedValue->clone();
    
    /* Set the copy arguments to their matches in the new DAG */
    const Environment& args     = function->getArguments();
    Environment&       copyArgs = const_cast<Environment&>( copy->function->getArguments() );
    
    for ( size_t i = 0; i < args.size(); i++ ) {
        const std::string &name = args.getName(i);
        
        // clone the parameter DAG node
        DAGNode* theArgClone = args[name].getDagNode()->cloneDAG(newNodes);
        copyArgs[name].setVariable(new Variable(theArgClone) );
  
        // this is perhaps not necessary because we already set the parent child relationship automatically
        copy->addParentNode( theArgClone );
        theArgClone->addChildNode( copy );
    }
    
    /* Make sure the children clone themselves */
    for( std::set<VariableNode*>::const_iterator i = children.begin(); i != children.end(); i++ ) {
        (*i)->cloneDAG( newNodes );
    }
    
    return copy;
}



/** Get affected nodes: touch and pass through to next stochastic node */
void DeterministicNode::getAffected( std::set<StochasticNode*>& affected ) {

    /* If we have already touched this node, we are done; otherwise, get the affected children */
    if ( !touched ) {
        touched = true;
        changed = false;
        for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ ) {
            (*i)->getAffected( affected );
        }
    }
}


/** Get class vector describing type of DAG node */
const VectorString& DeterministicNode::getClass() const {

    static VectorString rbClass = VectorString( DeterministicNode_name ) + VariableNode::getClass();
    return rbClass;
}


/** Get stored value */
const RbLanguageObject* DeterministicNode::getStoredValue( void ) {

    if ( !touched )
        return value;

    if ( !changed )
        update();
    
    return storedValue;
}


/** Get value */
const RbLanguageObject* DeterministicNode::getValue( void ) {

    if ( touched && !changed )
        update();

    return value;
}


/** Get value pointer */
RbLanguageObject* DeterministicNode::getValuePtr( void ) {
    
    if ( touched && !changed )
        update();
    
    return value;
}


/** Keep value of node and affected variable nodes */
void DeterministicNode::keep( void ) {

    keepAffected();
}


/** Keep value of node and affected variable nodes */
void DeterministicNode::keepAffected( void ) {

    if ( touched ) {
        if ( !changed )
            update();
        if ( storedValue != NULL) {
            storedValue->release();
            if (storedValue->isUnreferenced()) {
                delete storedValue;
            }
            storedValue = NULL;
        }
        for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ ) {
            (*i)->keepAffected();
        }
    }
    touched = changed = false;
}


/** Print value for user */
void DeterministicNode::printValue( std::ostream& o ) {

    if ( touched && !changed )
        update();

    if (value != NULL) 
        value->printValue(o);
}

/** Print struct for user */
void DeterministicNode::printStruct( std::ostream& o ) const {
    
    o << "_DAGClass    = " << getClass() << std::endl;
    o << "_value       = " << value->briefInfo() << std::endl;
    if ( touched && changed )
        o << "_storedValue = " << storedValue->briefInfo() << std::endl;
    
    o << "_valueType   = " << getValueType() << std::endl;
    o << "_function    = " << function->getType() << std::endl;
    o << "_touched     = " << ( touched ? Boolean( true ) : Boolean( false ) ) << std::endl;
    o << "_changed     = " << ( changed ? Boolean( true ) : Boolean( false ) ) << std::endl;
    
    o << "_parents     = ";
    printParents(o);
    o << std::endl;
    
    o << "_children    = ";
    printChildren(o);
    o << std::endl;
    
    o << std::endl;
}


/** Complete info about object */
std::string DeterministicNode::richInfo( void ) const {
    
    std::ostringstream o;
    
    o << "FunctionNode:" << std::endl;
    
    o << "touched     = " << (touched ? "true" : "false") << std::endl;
    o << "changed     = " << (changed ? "true" : "false") << std::endl;
    
    o << "function    = ";
    function->printValue(o);
    o << std::endl;
    
    o << "value       = ";
    value->printValue(o);
    o << std::endl;
    
    o << "storedValue = ";
    storedValue->printValue(o);
    o << std::endl;
    
    return o.str();
}



/** Restore value of node and affected variable nodes */
void DeterministicNode::restoreAffected( void ) {

    if ( touched ) {
        if ( changed ) {
            if (value != NULL) {
                value->release();
                if (value->isUnreferenced()) {
                    delete value;
                }
            }
            value       = storedValue;
            storedValue = NULL;
        }
        for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ ) {
            (*i)->restoreAffected();
        }
    }
    touched = changed = false;
}


/** Swap parent node */
void DeterministicNode::swapParentNode( DAGNode* oldParent, DAGNode* newParent ) {

    if ( parents.find( oldParent ) == parents.end() )
        throw RbException( "Node is not a parent" );

    oldParent->removeChildNode( this );
    newParent->addChildNode   ( this );
    removeParentNode( oldParent );
    addParentNode( newParent );

    touchAffected();
}


/** Tell affected nodes that upstream value has been reset */
void DeterministicNode::touchAffected( void ) {

    // only if this node is not touched we start touching all affected nodes
    // this pervents infinite recursion in statement like "a <- a + b"
    if (!touched) {
        touched = true;
        changed = false;
        for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ )
            (*i)->touchAffected();
    }
}

/** Update value and stored value after node and its surroundings have been touched by a move */
void DeterministicNode::update( void ) {
    
    if ( touched && !changed ) {
        
//        assert( storedValue == NULL );
        
        
        // set the stored value and release the old stored value
        if (storedValue != NULL) {
            storedValue->release();
            if (storedValue->isUnreferenced()) {
                delete storedValue;
            }
        }
        storedValue     = value;
        
        // compute a new value, set and retain it
        RbLanguageObject* retVal = function->execute();
        value           = retVal;
        value->retain();
        
        // mark as changed
        changed         = true;
        
    }
}


