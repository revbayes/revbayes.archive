/**
 * @file
 * This file contains the implementation of ConstantInferenceNode, which is derived
 * from InferenceDagNode. ConstantInferenceNode is used for DAG nodes holding constant
 * values.
 *
 * @brief Implementation of ConstantInferenceNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-10 10:31:21 +0200 (Tue, 10 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends ConstantInferenceNode
 *
 * $Id: ConstantInferenceNode.cpp 1370 2012-04-10 08:31:21Z hoehna $
 */

#include "ConstantInferenceNode.h"
#include "RbException.h"


/** Constructor from value class */
ConstantInferenceNode::ConstantInferenceNode( const RbValue<void*> &val ) : InferenceDagNode( val ) {
    
}

/** Copy constructor */
ConstantInferenceNode::ConstantInferenceNode( const ConstantInferenceNode &x ) : InferenceDagNode( x ) {
    value = x.value;
}


/** Destructor. We own the object, so we will delete it. */
ConstantInferenceNode::~ConstantInferenceNode(void) {
//    delete value;
}


/** 
 * Get the affected nodes.
 * This call is started by the parent and since we don't have one this is a dummy implementation!
 */
void ConstantInferenceNode::getAffected(std::set<StochasticInferenceNode* > &affected) {
    // do nothing
    throw RbException("You should never call getAffected() of a constant node!!!");
}


/**
 * Is this node eliminated.
 * No, constant nodes can not be eliminated.
 */
bool ConstantInferenceNode::isEliminated( void ) const {
    return false;
}

/**
 * Is this node eliminated.
 * No, constant nodes can not be eliminated.
 */
bool ConstantInferenceNode::isNotInstantiated( void ) const {
    return false;
}

/** Keep value of node */
void ConstantInferenceNode::keepMe( void ) {
    // do nothing
}


///** Print struct for user */
//void ConstantInferenceNode::printStruct(std::ostream &o) const {
//    
//    o << "_Class        = Constant inference node" << std::endl;
//    
//    o << "_children     = ";
//    printChildren(o);
//    o << std::endl;
//    
//    o << std::endl;
//}

/** Restore value of node */
void ConstantInferenceNode::restoreMe( void ) {
    // do nothing
}


/** Touch value of node */
void ConstantInferenceNode::touchMe( void ) {
    // do nothing
}
