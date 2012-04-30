/**
 * @file
 * This file contains the implementation of some function in DeterministicNode,
 * which is an abstract base class for DAG nodes associated with expressions
 * (equations) determining their value.
 *
 * @brief Partial implementation of DeterministicNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "ConstructorFunction.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "DeterministicInferenceNode.h"
#include "InferenceDagNode.h"
#include "InferenceFunction.h"
#include "Model.h"
#include "RbBoolean.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbMemoryManager.h"
#include "RbUtil.h"
#include "UserInterface.h"
#include "Workspace.h"

#include <algorithm>
#include <cassert>


/** Constructor of empty deterministic node */
DeterministicNode::DeterministicNode( void ) : VariableNode( ), needsUpdate( true ) {
}

/** Constructor of empty deterministic node */
DeterministicNode::DeterministicNode( RbFunction* func, const Plate *pl ) : VariableNode( pl ), needsUpdate( true ), function( func ) {
    
    /* Check for cycles */
    const std::vector<Argument>& arguments = func->getArguments();
    std::list<DAGNode*> done;
    for ( size_t i = 0; i < arguments.size(); i++ ) {
        if ( arguments[i].getVariable().getDagNode() != NULL && arguments[i].getVariable().getDagNode()->isParentInDAG( this, done ) )
            throw RbException( "Invalid assignment: cycles in the DAG" );
    }
    
    /* Set parents and add this node as a child node of these */
    for ( size_t i = 0; i < arguments.size(); i++ ) {
        DAGNode* theArgument = const_cast<Variable*>( (const Variable*)arguments[i].getVariablePtr())->getDagNode();
        addParentNode( theArgument );
        theArgument->addChildNode( this );
    }
    
    /* Set value and stored value */
    // \TODO: We should not allow const casts
    value           = const_cast<RbLanguageObject*>( &function->execute() );
    storedValue     = NULL;
    
    needsUpdate     = false;
    
}


/** Copy constructor */
DeterministicNode::DeterministicNode( const DeterministicNode& x ) : VariableNode( x ) {
    
    function        = x.function->clone();
    touched         = x.touched;
    needsUpdate     = x.needsUpdate;
    // We do not own the stored value, but the function does
    storedValue     = x.storedValue;
    
    /* Set parents and add this node as a child node of these */
    std::vector<Argument>& args = function->getArguments();
    for ( size_t i = 0; i < args.size(); i++ ) {
        
        DAGNode* theArgument = const_cast<DAGNode*>( args[i].getVariable().getDagNode() );
        addParentNode( theArgument );
        theArgument->addChildNode( this );
    }
}


/** Destructor */
DeterministicNode::~DeterministicNode( void ) {

    delete function;
}

DeterministicNode* DeterministicNode::clone() const {
    return new DeterministicNode(*this);
}


/** 
 *  Calculate the summed over ln-probability.
 *  We just take the product of the probability of our children.
 */
double DeterministicNode::calculateEliminatedLnProbability( bool enforceProbabilityCalculation ) {
    // initialize the log probability
    double lnProb = 0.0;
    
    // our probability is the product of the probabilities of our children
    for (std::set<VariableNode*>::iterator child = children.begin(); child != children.end(); child++) {
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
void DeterministicNode::constructSumProductSequence(std::set<VariableNode *> &nodes, std::vector<StochasticNode*>& sequence) {
    // if I was added already, then I'm done
    if ( nodes.find( this ) == nodes.end() ) {
        nodes.insert( this );
        
        // test whether I'm actually eliminated
        if ( isNotInstantiated() ) {
            // if so, add my parents
            
            // first the parents
            for (std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); i++) {
                if ( (*i)->isNotInstantiated() ) {
                    static_cast<VariableNode*>( *i )->constructSumProductSequence(nodes,sequence);
                }
            }
        }
        
        // test whether I'm actually eliminated
        if ( isNotInstantiated() ) {
            // if so, add my children
            
            // then the children
            for (std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++) {
                static_cast<VariableNode*>( *i )->constructSumProductSequence(nodes,sequence);
            }
        }
    }
    
}


/** Clone the entire graph: clone children, swap parents */
DAGNode* DeterministicNode::cloneDAG( std::map<const DAGNode*, RbDagNodePtr>& newNodes ) const {
    
    if ( newNodes.find( this ) != newNodes.end() )
        return newNodes[ this ];
    
    /* Get pristine copy */
    DeterministicNode* copy = new DeterministicNode(  );
    newNodes[ this ] = copy;
    
    /* Set the name so that the new node remains identifiable */
    copy->setName(name);
    
    /* Set the copy member variables */
    copy->function      = function->clone();
    copy->touched       = touched;
    copy->needsUpdate   = needsUpdate;
    // We do not own the value so we do not need to clone it
    copy->value         = value;
    copy->storedValue   = storedValue;
    
    /* Set the copy arguments to their matches in the new DAG */
    std::vector<Argument>& args      = function->getArguments();
    
    // clear the copy arguments
    copy->function->clear();
    
    for ( size_t i = 0; i < args.size(); i++ ) {
        
        // clone the parameter DAG node
        DAGNode* theArgClone = args[i].getVariable().getDagNode()->cloneDAG(newNodes);
        // \TODO: I don't think that we should create a new variable. That will destroy our dependencies structure.
//        copyArgs[i].getVariable().setDagNode( theArgClone );
        copy->function->setArgument(args[i].getLabel(), Argument( RbVariablePtr( new Variable( theArgClone ) ) ) );
  
        // this is perhaps not necessary because we already set the parent child relationship automatically
        copy->addParentNode( theArgClone );
        theArgClone->addChildNode( copy );
    }
    
    /* Make sure the children clone themselves */
    for( std::set<VariableNode*>::const_iterator i = children.begin(); i != children.end(); i++ ) {
        VariableNode* theNewNode = *i;
        // do not add myself into the list of nodes
        if ( theNewNode->isTypeSpec( DeterministicNode::getClassTypeSpec() ) ) {
            DeterministicNode* theDetNode = dynamic_cast<DeterministicNode*>(theNewNode);
            const RbFunction& theFunction = theDetNode->getFunction();
            if (theFunction.isTypeSpec(ConstructorFunction::getClassTypeSpec())) {
                const ConstructorFunction& theConstructorFunction = dynamic_cast<const ConstructorFunction&>( theFunction );
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
 * A deterministic node create a lean constant DAG node and sets the value. 
 * Additionally we call the create lean DAG for all children and all parents.
 * If we already have a lean copy of this deterministic node, we will just return that copy
 * and not call our children and parents.
 */
InferenceDagNode* DeterministicNode::createLeanDag(std::map<const DAGNode *, InferenceDagNode *> &newNodes) const {
    
    if ( newNodes.find( this ) != newNodes.end() )
        return ( newNodes[ this ] );
    
    // create a copy of the inference function
    InferenceFunction* leanFunction = function->getLeanFunction()->clone();
    
    // make a copy of the current value
    RbValue<void*> leanValue;
    leanValue.value = value->getLeanValue( leanValue.lengths );
    
    /* Create a lean DAG node */
    DeterministicInferenceNode* copy = new DeterministicInferenceNode( leanValue, leanFunction, name);
    newNodes[ this ] = copy;
    
    /* Set the copy arguments to their matches in the new DAG */
    std::vector<Argument>& args      = function->getArguments();
    
    std::vector<RbValue<void*> > leanArgs;
    for ( size_t i = 0; i < args.size(); i++ ) {
        
        // clone the parameter DAG node
        InferenceDagNode* theArgClone = args[i].getVariable().getDagNode()->createLeanDag(newNodes);
        leanArgs.push_back( theArgClone->getValue() );
        
        // this is perhaps not necessary because we already set the parent child relationship automatically
        copy->addParentNode( theArgClone );
        theArgClone->addChildNode( copy );
    }
    
    // set the value of the function to the args
    leanArgs.push_back( leanValue );
    
    // link lean args with inference function
    leanFunction->setArguments( leanArgs );
    
    /* Make sure the children create a lean copy of themselves too */
    for( std::set<VariableNode* >::const_iterator i = children.begin(); i != children.end(); i++ ) {
        (*i)->createLeanDag( newNodes );
    }
    
    return copy;
}


/** Complete info about object */
std::string DeterministicNode::debugInfo( void ) const {
    
    std::ostringstream o;
    
    o << "Deterministic Node:" << std::endl;
    
    o << "touched     = " << (touched ? "true" : "false") << std::endl;
    o << "needsUpdate = " << (needsUpdate ? "true" : "false") << std::endl;
    
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



/** Get affected nodes: pass through to next stochastic node */
void DeterministicNode::getAffected( std::set<StochasticNode* >& affected ) {

    // if this node is eliminated, then we just add the factor root and that's it
    if ( isNotInstantiated() ) {
        if ( factorRoot == NULL ) {
            std::cerr << "Uhh Ohhh";
        }
        affected.insert( factorRoot );
    }
    else {
        // we add all our children
        for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ ) {
            (*i)->getAffected( affected );
        }
    }
}


/** Get class name of object */
const std::string& DeterministicNode::getClassName(void) { 
    
    static std::string rbClassName = "Deterministic DAG node";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& DeterministicNode::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( VariableNode::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& DeterministicNode::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



const RbFunction& DeterministicNode::getFunction(void) const {
    return *function;
}



/** Get stored value */
const RbLanguageObject& DeterministicNode::getStoredValue( void ) const {

    if ( !touched )
        return *value;
    
    return *storedValue;
}


/** Get value */
const RbLanguageObject& DeterministicNode::getValue( void ) const {
    
    if ( touched && needsUpdate )
        const_cast<DeterministicNode*>(this)->update();
    
    return *value;
}


/** Get value */
RbLanguageObject& DeterministicNode::getValue( void ) {
    
    //throw RbException("We should never call non-const getValue in deterministic nodes because we don't own them.");
    
    if ( touched && needsUpdate )
        update();
    
    return *value;
}


///** Get value pointer */
//const RbLanguageObject* DeterministicNode::getValuePtr( void ) const {
//    
//    if ( touched && needsUpdate )
//        const_cast<DeterministicNode*>(this)->update();
//    
//    return value;
//}

/** 
 * Is this node eliminated?
 * If at least one parent is eliminated, then this node is also eliminated.
 */
bool DeterministicNode::isEliminated( void ) const {
    // ask all parents
    for (std::set<DAGNode*>::const_iterator i = parents.begin(); i != parents.end(); i++) {
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
bool DeterministicNode::isNotInstantiated( void ) const {
    // ask all parents
    for (std::set<DAGNode*>::const_iterator i = parents.begin(); i != parents.end(); i++) {
        if ( (*i)->isNotInstantiated() ) {
            return true;
        }
    }
    
    // non of the parents was eleminated, so this node isn't either
    return false;
}


/** Keep value of node and affected variable nodes */
void DeterministicNode::keepMe( void ) {
    
    if ( touched ) {
        
        storedValue = NULL;
        
        if ( needsUpdate )
            update();
        
        //  I need to tell all my eliminated parents that they need to restore their likelihoods
        for (std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); ++i) {
            if ( (*i)->isNotInstantiated() ) {
                // since only variable nodes can be eliminated
                static_cast<VariableNode*>( *i )->keepMe();
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
void DeterministicNode::likelihoodsNeedUpdates() {
    
    // we need to mark this node as dirty so that it will recompute its likelihood and probability
    if (!touched) {
        // Store the current lnProb 
        touched      = true;
        
    }
    
    //  I need to tell all my eliminated parents that they need to update their likelihoods
    for (std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); i++) {
        if ( (*i)->isNotInstantiated() ) {
            // since only variable nodes can be eliminated
            static_cast<VariableNode*>( *i )->likelihoodsNeedUpdates();
        }
    }
}


/** 
 * Mark this deterministic node for recalculation.
 * We set our flag four updating the value.
 * We delegate to all our children that they need to recalculate their probabilities.
 */
void DeterministicNode::markForRecalculation(void) {
    // set our flag for update
    needsUpdate = true;
    
    // tell all children to recalculate their values
    markChildrenForRecalculation();
}


/**
 * Does the probability or likelihood of this node needs updateing?
 * We need to propagate this call to all our children.
 */
bool DeterministicNode::needsRecalculation() const {
    
    //  I need to ask all my children if they need to recalculate their probabilities or likelihoods
    for (std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++) {
        if ( (*i)->needsRecalculation() ) {
            return true;
        }
    }
    
    return false;
}


/** Print value for user */
void DeterministicNode::printValue( std::ostream& o ) const {

    if ( touched && needsUpdate )
        const_cast<DeterministicNode*>(this)->update();

    if (value != NULL) 
        value->printValue(o);
}

/** Print struct for user */
void DeterministicNode::printStruct( std::ostream& o ) const {
    
    o << "_DAGClass    = " << getClassTypeSpec() << std::endl;
    o << "_value       = ";
    value->printValue(o);
    o << std::endl;
    if ( touched && !needsUpdate ) {
        o << "_storedValue = ";
        storedValue->printValue(o);
        o << std::endl;
    }
    
    o << "_valueType   = " << value->getTypeSpec() << std::endl;
    o << "_function    = " << function->getTypeSpec() << std::endl;
    o << "_touched     = " << ( touched ? RbBoolean( true ) : RbBoolean( false ) ) << std::endl;
    o << "_needsUpdate = " << ( needsUpdate ? RbBoolean( true ) : RbBoolean( false ) ) << std::endl;
    
    o << "_parents     = ";
    printParents(o);
    o << std::endl;
    
    o << "_children    = ";
    printChildren(o);
    o << std::endl;
    
    o << std::endl;
}


/** 
 * Restore value of node
 * We also need to restore the affected variable nodes */
void DeterministicNode::restoreMe( void ) {

    if ( touched ) {
        if (storedValue != NULL) {
//            delete value;
        
            // no matter if this node has been changed we just set it back to its stored value
            value       = storedValue;
        
            storedValue = NULL;
        }
        
        // restore all children
        for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ ) {
            (*i)->restoreMe();
        }
        
        //  I need to tell all my eliminated parents that they need to restore their likelihoods
        for (std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); ++i) {
            if ( (*i)->isNotInstantiated() ) {
                // since only variable nodes can be eliminated
                static_cast<VariableNode*>( *i )->restoreMe();
            }
        }
    }
    touched = false;
}


/** Swap parent node */
void DeterministicNode::swapParentNode( DAGNode* oldParent, DAGNode* newParent ) {

    if ( parents.find( oldParent ) == parents.end() )
        throw RbException( "Node is not a parent" );

    oldParent->removeChildNode( this );
    newParent->addChildNode   ( this );
    removeParentNode( oldParent );
    addParentNode( newParent );

    touch();
}


/** Tell affected nodes that upstream value has been reset */
void DeterministicNode::touchMe( void ) {
    
    // only if this node is not touched we start touching all affected nodes
    if (!touched) {
        // flag myself as being touched
        touched     = true;
        
        // store the current value; this should happen only by the first touch unless we change the stored values into a stack
        storedValue = value;
        value = NULL;
    }
        
    // flag myself for an update
    needsUpdate = true;
    
    // touch all my children because they are affected too
    for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->touchMe();
    
    
    //  I need to tell all my eliminated parents that they need to update their likelihoods
    for (std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); i++) {
        if ( (*i)->isNotInstantiated() ) {
            // since only variable nodes can be eliminated
            static_cast<VariableNode*>( *i )->likelihoodsNeedUpdates();
        }
    }
    
}

/** Update value and stored value after node and its surroundings have been touched by a move */
void DeterministicNode::update( void ) {
    
    if ( touched && needsUpdate ) {
        
//        assert( storedValue == NULL );
        
//        // set the stored value and release the old stored value
//        storedValue     = value;
        
        // compute a new value
        // \TODO: We should not allow const casts
        value = const_cast<RbLanguageObject*>( &function->execute() );
        
        // mark as changed
        needsUpdate = false;
        
    }
}


