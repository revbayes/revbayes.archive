/**
 * @file
 * This file contains the implementation of ConstantNode, which is derived
 * from DAGNode. ConstantNode is used for DAG nodes holding constant
 * values other.
 *
 * @brief Implementation of ConstantNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends ConstantNode
 *
 * $Id$
 */


#include "ConstantInferenceNode.h"
#include "ConstantNode.h"
#include "ConstructorFunction.h"
#include "DeterministicNode.h"
#include "InferenceDagNode.h"
#include "Model.h"
#include "RbException.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "Workspace.h"

#include <cassert>
#include <iostream>
#include <string>
#include <set>


/** Constructor from value */
ConstantNode::ConstantNode( RbLanguageObject* val ) : DAGNode( ) {
    value = val;
}


/** Constructor from value class */
ConstantNode::ConstantNode( void ) : DAGNode( ) {

}

/** Copy constructor */
ConstantNode::ConstantNode( const ConstantNode &x ) : DAGNode( x ) {
    value = x.value->clone();
}


/** Destructor. We own the object, so we will delete it. */
ConstantNode::~ConstantNode(void) {
    delete value;
}

/** Clone this object */
ConstantNode* ConstantNode::clone( void ) const {

    return new ConstantNode( *this );
}


/** Cloning the entire graph only involves children for a constant node */
DAGNode* ConstantNode::cloneDAG( std::map<const DAGNode*, RbDagNodePtr>& newNodes ) const {

    if ( newNodes.find( this ) != newNodes.end() )
        return ( newNodes[ this ] );

    /* Make pristine copy */
    ConstantNode* copy = clone();
    newNodes[ this ] = copy;
    
    /* Make sure the children clone themselves */
    for( std::set<VariableNode* >::const_iterator i = children.begin(); i != children.end(); i++ ) {
        VariableNode* theNewNode = *i;
        // do not add myself into the list of nodes
        if ( theNewNode->isTypeSpec( DeterministicNode::getClassTypeSpec() ) ) {
            DeterministicNode* theDetNode = dynamic_cast<DeterministicNode*>(theNewNode);
            const RbFunction& theFunction = theDetNode->getFunction();
            if (theFunction.isTypeSpec(ConstructorFunction::getClassTypeSpec())) {
                const ConstructorFunction& theConstructorFunction = static_cast<const ConstructorFunction&>( theFunction );
                if ( theConstructorFunction.getReturnType() == Model::getClassTypeSpec() ) {
                    continue;
                }
            }
        }
        (*i)->cloneDAG( newNodes );
    }
 
    return copy;
}


/**
 * A constant node only create a lean constant DAG node and sets the value. 
 * Additionally we call the create lean DAG for all children.
 * If we already have a lean copy of this constant node, we will just return that copy
 * and not call our children.
 */
InferenceDagNode* ConstantNode::createLeanDag(std::map<const DAGNode *, InferenceDagNode *> &newNodes) const {
    
    if ( newNodes.find( this ) != newNodes.end() )
        return ( newNodes[ this ] );
    
    // make a copy of the current value
    RbValue<void*> leanValue;
    leanValue.value = value->getLeanValue( leanValue.lengths );
    
    /* Create a lean DAG node */
    ConstantInferenceNode* copy = new ConstantInferenceNode( leanValue, name );
    newNodes[ this ] = copy;
    
    /* Make sure the children create a lean copy of themselves too */
    for( std::set<VariableNode* >::const_iterator i = children.begin(); i != children.end(); i++ ) {
        (*i)->createLeanDag( newNodes );
    }
    
    return copy;
}


/** 
 * Get the affected nodes.
 * This call is started by the parent and since we don't have one this is a dummy implementation!
 */
void ConstantNode::getAffected(std::set<StochasticNode* > &affected) {
    // do nothing
    throw RbException("You should never call getAffected() of a constant node!!!");
}



/** Get class name of object */
const std::string& ConstantNode::getClassName(void) { 
    
    static std::string rbClassName = "Constant DAG node";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& ConstantNode::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( DAGNode::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& ConstantNode::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


const RbLanguageObject& ConstantNode::getStoredValue(void) const {
    return *value;
}


const RbLanguageObject& ConstantNode::getValue(void) const {
    return *value;
}


RbLanguageObject& ConstantNode::getValue(void) {
    return *value;
}


//const RbLanguageObject* ConstantNode::getValuePtr(void) const {
//    return value;
//}

/**
 * Is this node eliminated.
 * No, constant nodes can not be eliminated.
 */
bool ConstantNode::isEliminated( void ) const {
    return false;
}

/**
 * Is this node eliminated.
 * No, constant nodes can not be eliminated.
 */
bool ConstantNode::isNotInstantiated( void ) const {
    return false;
}

/** Keep value of node */
void ConstantNode::keepMe( void ) {
    // do nothing
}

/** Print value for user */
void ConstantNode::printValue( std::ostream& o ) const {

    if ( value != NULL )
        value->printValue(o);
    else  /* NULL value */
        o << "NULL";
}


/** Print struct for user */
void ConstantNode::printStruct(std::ostream &o) const {

    o << "_Class        = " << getClassTypeSpec() << std::endl;
    o << "_valueType    = " << value->getTypeSpec() << std::endl;
    o << "_value        = ";
    value->printValue(o);
    o << std::endl;

    o << "_children     = ";
    printChildren(o);
    o << std::endl;

    o << std::endl;
}

/** Restore value of node */
void ConstantNode::restoreMe( void ) {
    // do nothing
}

/**
 * Set value: same as clamp, but do not clamp. This function will
 * also be used by moves to propose a new value.
 */
void ConstantNode::setValue( RbLanguageObject* val ) {
    
    if (val == NULL) {
        std::cerr << "Ooops ..." << std::endl;
    }
    
    // touch the node (which will store the lnProb)
    touch();
    
    // free the memory of the old value
    if (value != NULL) {
        delete value;
    }
    // set the value
    value = val;
}

/** Touch value of node */
void ConstantNode::touchMe( void ) {
    // do nothing
}

