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
#include "RbBoolean.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbMemoryManager.h"
#include "RbUtil.h"
#include "UserInterface.h"
#include "VectorString.h"
#include "Workspace.h"

#include <algorithm>
#include <cassert>


// Definition of the static type spec member
const TypeSpec DeterministicNode::typeSpec(DeterministicNode_name);

/** Constructor of empty deterministic node */
DeterministicNode::DeterministicNode( const std::string& valType ) : VariableNode( valType ), changed( false ) {
}

/** Constructor of empty deterministic node */
DeterministicNode::DeterministicNode( RbPtr<RbFunction> func ) : VariableNode(func->getReturnType()), changed( false ) {
    
    // increment the reference count for myself
    RbMemoryManager::rbMemoryManager().incrementCountForAddress(this);
    
    /* Check for cycles */
    RbPtr<Environment> arguments = func->getArguments();
    std::list<DAGNode*> done;
    for ( size_t i = 0; i < arguments->size(); i++ ) {
        const std::string &name = arguments->getName(i);
        if ( (*arguments)[name]->getDagNode() != NULL && (*arguments)[name]->getDagNode()->isParentInDAG( RbPtr<DAGNode>(this), done ) )
            throw RbException( "Invalid assignment: cycles in the DAG" );
    }
    
    /* Set parents and add this node as a child node of these */
    for ( size_t i = 0; i < arguments->size(); i++ ) {
        const std::string &name = arguments->getName(i);
        RbPtr<DAGNode> theArgument = (*arguments)[name]->getDagNode();
        addParentNode( theArgument );
        theArgument->addChildNode( this );
    }
    
    /* Set the function */
    function = func;
    
    /* Set value and stored value */
    RbPtr<RbLanguageObject> retVal = function->execute();
    
    value           = retVal;
    storedValue     = RbPtr<RbLanguageObject>::getNullPtr();
    
    // decrement the reference count for myself
    RbMemoryManager::rbMemoryManager().decrementCountForAddress(this);
}


/** Copy constructor */
DeterministicNode::DeterministicNode( const DeterministicNode& x ) : VariableNode( x ) {
    
    function         = RbPtr<RbFunction>( x.function->clone() );
    touched          = x.touched;
    changed          = x.changed;
    if ( x.storedValue != NULL ) {
        storedValue  = RbPtr<RbLanguageObject>( x.storedValue->clone() );
    }
    else
        storedValue  = RbPtr<RbLanguageObject>::getNullPtr();
    
    /* Set parents and add this node as a child node of these */
    RbPtr<Environment> args = function->getArguments();
    for ( size_t i = 0; i < args->size(); i++ ) {
        const std::string &name = args->getName(i);
        
        RbPtr<DAGNode> theArgument = (*args)[name]->getDagNode();
        addParentNode( theArgument );
        theArgument->addChildNode( this );
    }
}


/** Destructor */
DeterministicNode::~DeterministicNode( void ) {
    
    /* Remove parents first so that VariableNode destructor does not get in the way */
    for ( std::set<RbPtr<DAGNode> >::iterator i = parents.begin(); i != parents.end(); i++ ) {
        RbPtr<DAGNode> node = *i;
        node->removeChildNode(this);
        removeParentNode(node);
    }
    parents.clear();

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
    copy->function = RbPtr<RbFunction>( function->clone() );
    copy->touched  = touched;
    copy->changed  = changed;
    if (value != NULL)
        copy->value    = RbPtr<RbLanguageObject>( value->clone() );
    if (storedValue == NULL)
        copy->storedValue = RbPtr<RbLanguageObject>::getNullPtr();
    else
        copy->storedValue = RbPtr<RbLanguageObject>( storedValue->clone() );
    
    /* Set the copy arguments to their matches in the new DAG */
    RbPtr<const Environment> args     = function->getArguments();
    RbPtr<Environment> copyArgs       = ( copy->function->getArguments() );
    
    for ( size_t i = 0; i < args->size(); i++ ) {
        const std::string &name = args->getName(i);
        
        // clone the parameter DAG node
        RbPtr<DAGNode> theArgClone( (*args)[name]->getDagNode()->cloneDAG(newNodes) );
        (*copyArgs)[name]->setVariable(RbPtr<Variable>( new Variable(theArgClone) ) );
  
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
void DeterministicNode::getAffected( std::set<RbPtr<StochasticNode> >& affected ) {

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


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& DeterministicNode::getTypeSpec(void) const {
    return typeSpec;
}



RbPtr<const RbFunction> DeterministicNode::getFunction(void) const {
    return RbPtr<const RbFunction>(function);
}


RbPtr<RbFunction> DeterministicNode::getFunction(void) {
    return (function);
}



/** Get stored value */
RbPtr<const RbLanguageObject> DeterministicNode::getStoredValue( void ) const {

    if ( !touched )
        return RbPtr<const RbLanguageObject>(value);
    
    return RbPtr<const RbLanguageObject>(storedValue);
}


/** Get value */
RbPtr<const RbLanguageObject> DeterministicNode::getValue( void ) const {
    
    if ( touched && !changed )
        const_cast<DeterministicNode*>(this)->update();
    
    return RbPtr<const RbLanguageObject>(value);
}

/** Get value */
RbPtr<RbLanguageObject> DeterministicNode::getValue( void ) {
    
    if ( touched && !changed )
        update();
    
    return RbPtr<RbLanguageObject>(value);
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
        
        storedValue = RbPtr<RbLanguageObject>::getNullPtr();
        
        for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ ) {
            (*i)->keepAffected();
        }
    }
    touched = changed = false;
}


/** Print value for user */
void DeterministicNode::printValue( std::ostream& o ) const {

    if ( touched && !changed )
        const_cast<DeterministicNode*>(this)->update();

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
    o << "_touched     = " << ( touched ? RbBoolean( true ) : RbBoolean( false ) ) << std::endl;
    o << "_changed     = " << ( changed ? RbBoolean( true ) : RbBoolean( false ) ) << std::endl;
    
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
    
    o << "Deterministic Node:" << std::endl;
    
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
            value       = storedValue;
            storedValue = RbPtr<RbLanguageObject>::getNullPtr();
        }
        for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ ) {
            (*i)->restoreAffected();
        }
    }
    touched = changed = false;
}


/** Swap parent node */
void DeterministicNode::swapParentNode( RbPtr<DAGNode> oldParent, RbPtr<DAGNode> newParent ) {

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
        storedValue     = value;
        
        // compute a new value, set and retain it
        value = function->execute();
        
        // mark as changed
        changed         = true;
        
    }
}


