/**
 * @file
 * This file contains the implementation of some function in DeterministicInferenceNode,
 * which is an abstract base class for DAG nodes associated with expressions
 * (equations) determining their value.
 *
 * @brief Partial implementation of DeterministicInferenceNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-24 20:57:15 -0700 (Tue, 24 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: DeterministicInferenceNode.cpp 1455 2012-04-25 03:57:15Z hoehna $
 */

#include "DeterministicInferenceNode.h"
#include "InferenceFunction.h"

#include <cassert>
#include <iostream>

/** Constructor of empty deterministic node */
DeterministicInferenceNode::DeterministicInferenceNode( const RbValue<void*> &v, InferenceFunction* func ) : VariableInferenceNode( v ), needsUpdate( true ), function( func ) {
    
    
    
}


/** Copy constructor */
DeterministicInferenceNode::DeterministicInferenceNode( const DeterministicInferenceNode& x ) : VariableInferenceNode( x ) {
    
    
}


/** Destructor */
DeterministicInferenceNode::~DeterministicInferenceNode( void ) {
    
    delete function;
}


/** 
 *  Calculate the summed over ln-probability.
 *  We just take the product of the probability of our children.
 */
double DeterministicInferenceNode::calculateEliminatedLnProbability( bool enforceProbabilityCalculation ) {
    // initialize the log probability
    double lnProb = 0.0;
    
    // our probability is the product of the probabilities of our children
    for (std::set<VariableInferenceNode*>::iterator child = children.begin(); child != children.end(); child++) {
        lnProb += (*child)->calculateEliminatedLnProbability( enforceProbabilityCalculation );
    }
    
    return lnProb;
}


/**
 * Construct the set of nodes which are not instantiated.
 *
 * I need to add all nodes which are either:
 * a) not instantiated, or
 * b) have not instantiated parents
 */
void DeterministicInferenceNode::constructSumProductSequence(std::set<VariableInferenceNode *> &nodes, std::vector<StochasticInferenceNode*>& sequence) {
    // if I was added already, then I'm done
    if ( nodes.find( this ) == nodes.end() ) {
        nodes.insert( this );
        
        // test whether I'm actually eliminated
        if ( isNotInstantiated() ) {
            // if so, add my parents
            
            // first the parents
            for (std::set<InferenceDagNode*>::iterator i = parents.begin(); i != parents.end(); i++) {
                if ( (*i)->isNotInstantiated() ) {
                    static_cast<VariableInferenceNode*>( *i )->constructSumProductSequence(nodes,sequence);
                }
            }
        }
        
        // test whether I'm actually eliminated
        if ( isNotInstantiated() ) {
            // if so, add my children
            
            // then the children
            for (std::set<VariableInferenceNode*>::iterator i = children.begin(); i != children.end(); i++) {
                static_cast<VariableInferenceNode*>( *i )->constructSumProductSequence(nodes,sequence);
            }
        }
    }
    
}


/** Get affected nodes: pass through to next stochastic node */
void DeterministicInferenceNode::getAffected( std::set<StochasticInferenceNode* >& affected ) {
    
    // if this node is eliminated, then we just add the factor root and that's it
    if ( isNotInstantiated() ) {
        if ( factorRoot == NULL ) {
            std::cerr << "Uhh Ohhh";
        }
        affected.insert( factorRoot );
    }
    else {
        // we add all our children
        for ( std::set<VariableInferenceNode*>::iterator i = children.begin(); i != children.end(); i++ ) {
            (*i)->getAffected( affected );
        }
    }
}



InferenceFunction* DeterministicInferenceNode::getFunction( void ) const {
    return function;
}


/** 
 * Is this node eliminated?
 * If at least one parent is eliminated, then this node is also eliminated.
 */
bool DeterministicInferenceNode::isEliminated( void ) const {
    // ask all parents
    for (std::set<InferenceDagNode*>::const_iterator i = parents.begin(); i != parents.end(); i++) {
        if ( (*i)->isEliminated() ) {
            return true;
        }
    }
    
    // non of the parents was eleminated, so this node isn't either
    return false;
}

/** 
 * Is this node eliminated?
 * If at least one parent is not instantiated, then this node is also eliminated.
 */
bool DeterministicInferenceNode::isNotInstantiated( void ) const {
    // ask all parents
    for (std::set<InferenceDagNode*>::const_iterator i = parents.begin(); i != parents.end(); i++) {
        if ( (*i)->isNotInstantiated() ) {
            return true;
        }
    }
    
    // non of the parents was eleminated, so this node isn't either
    return false;
}


/** Keep value of node and affected variable nodes */
void DeterministicInferenceNode::keepMe( void ) {
    
    if ( touched ) {
        
        // @TODO: call move and function
        function->keep();
        
        if ( needsUpdate )
            update();
        
        //  I need to tell all my eliminated parents that they need to restore their likelihoods
        for (std::set<InferenceDagNode*>::iterator i = parents.begin(); i != parents.end(); ++i) {
            if ( (*i)->isNotInstantiated() ) {
                // since only variable nodes can be eliminated
                static_cast<VariableInferenceNode*>( *i )->keepMe();
            }
        }
        
    }
    touched = false;
    
    keepAffected();
    
}


/**
 * The likelihoods for this node need to be updated. This should only be ever called if this node is eliminated, i.e. at least one parent is eliminated.
 * Since this is a deterministic node, we just tell it our eliminated parents.
 */
void DeterministicInferenceNode::likelihoodsNeedUpdates() {
    
    // we need to mark this node as dirty so that it will recompute its likelihood and probability
    if (!touched) {
        // Store the current lnProb 
        touched      = true;
        
    }
    
    //  I need to tell all my eliminated parents that they need to update their likelihoods
    for (std::set<InferenceDagNode*>::iterator i = parents.begin(); i != parents.end(); i++) {
        if ( (*i)->isNotInstantiated() ) {
            // since only variable nodes can be eliminated
            static_cast<VariableInferenceNode*>( *i )->likelihoodsNeedUpdates();
        }
    }
}


/** 
 * Mark this deterministic node for recalculation.
 * We set our flag four updating the value.
 * We delegate to all our children that they need to recalculate their probabilities.
 */
void DeterministicInferenceNode::markForRecalculation(void) {
    // set our flag for update
    needsUpdate = true;
    
    // tell all children to recalculate their values
    markChildrenForRecalculation();
}


/**
 * Does the probability or likelihood of this node needs updateing?
 * We need to propagate this call to all our children.
 */
bool DeterministicInferenceNode::needsRecalculation() const {
    
    //  I need to ask all my children if they need to recalculate their probabilities or likelihoods
    for (std::set<VariableInferenceNode*>::iterator i = children.begin(); i != children.end(); i++) {
        if ( (*i)->needsRecalculation() ) {
            return true;
        }
    }
    
    return false;
}

///** Print struct for user */
//void DeterministicInferenceNode::printStruct( std::ostream& o ) const {
//    
//    o << "_DAGClass    = " << getClassTypeSpec() << std::endl;
//    o << "_value       = ";
//    value->printValue(o);
//    o << std::endl;
//    if ( touched && !needsUpdate ) {
//        o << "_storedValue = ";
//        storedValue->printValue(o);
//        o << std::endl;
//    }
//    
//    o << "_valueType   = " << value->getTypeSpec() << std::endl;
//    o << "_function    = " << function->getTypeSpec() << std::endl;
//    o << "_touched     = " << ( touched ? RbBoolean( true ) : RbBoolean( false ) ) << std::endl;
//    o << "_needsUpdate = " << ( needsUpdate ? RbBoolean( true ) : RbBoolean( false ) ) << std::endl;
//    
//    o << "_parents     = ";
//    printParents(o);
//    o << std::endl;
//    
//    o << "_children    = ";
//    printChildren(o);
//    o << std::endl;
//    
//    o << std::endl;
//}


/** 
 * Restore value of node
 * We also need to restore the affected variable nodes */
void DeterministicInferenceNode::restoreMe( void ) {
    
    if ( touched ) {
        // @TODO: call move and function
        
        // restore all children
        for ( std::set<VariableInferenceNode*>::iterator i = children.begin(); i != children.end(); i++ ) {
            (*i)->restoreMe();
        }
        
        //  I need to tell all my eliminated parents that they need to restore their likelihoods
        for (std::set<InferenceDagNode*>::iterator i = parents.begin(); i != parents.end(); ++i) {
            if ( (*i)->isNotInstantiated() ) {
                // since only variable nodes can be eliminated
                static_cast<VariableInferenceNode*>( *i )->restoreMe();
            }
        }
    }
    touched = false;
}


/** Tell affected nodes that upstream value has been reset */
void DeterministicInferenceNode::touchMe( void ) {
    
    // only if this node is not touched we start touching all affected nodes
    if (!touched) {
        // flag myself as being touched
        touched     = true;
    }
    
    // flag myself for an update
    needsUpdate = true;
    
    // touch all my children because they are affected too
    for ( std::set<VariableInferenceNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->touchMe();
    
    
    //  I need to tell all my eliminated parents that they need to update their likelihoods
    for (std::set<InferenceDagNode*>::iterator i = parents.begin(); i != parents.end(); i++) {
        if ( (*i)->isNotInstantiated() ) {
            // since only variable nodes can be eliminated
            static_cast<VariableInferenceNode*>( *i )->likelihoodsNeedUpdates();
        }
    }
    
}

/** Update value and stored value after node and its surroundings have been touched by a move */
void DeterministicInferenceNode::update( void ) {
    
    if ( touched && needsUpdate ) {
        
        // compute a new value
        function->execute();
        
        // mark as changed
        needsUpdate = false;
        
    }
}


