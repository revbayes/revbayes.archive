/**
 * @file
 * This file contains the implementation of StochasticNode, which is derived
 * from VariableNode. StochasticNode is used for DAG nodes holding stochastic
 * variables with an associated distribution object.
 *
 * @brief Implementation of StochasticNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2010-01-12 21:51:54 +0100 (Tis, 12 Jan 2010) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: StochasticNode.cpp 221 2010-01-12 20:51:54Z Hoehna $
 */

#include "RbBoolean.h"
#include "Distribution.h"
#include "DistributionDiscrete.h"
#include "RbException.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "UserInterface.h"
#include "VectorString.h"
#include "Workspace.h"

#include <algorithm>
#include <cassert>


// Definition of the static type spec member
const TypeSpec StochasticNode::typeSpec(StochasticNode_name);

/** Constructor of empty StochasticNode */
StochasticNode::StochasticNode( const TypeSpec& typeSp ) : VariableNode( typeSp.getType() ), clamped( false ), distribution( NULL ), instantiated( true ), needsRecalculation( true ), storedValue( NULL ) {
}


/** Constructor from distribution */
StochasticNode::StochasticNode( Distribution* dist ) : VariableNode( dist->getVariableType() ), clamped( false ), distribution( dist ), instantiated( true ), needsRecalculation( true ), storedValue( NULL ) {

    // increment the reference count for myself
    RbDagNodePtr::incrementCountForAddress(this);
    
    /* Get distribution parameters */
    Environment& params = dist->getMembers();

    /* Check for cycles */
    std::list<DAGNode*> done;
    for ( size_t i = 0; i < params.size(); i++ ) {
        done.clear();
        const std::string &name = params.getName(i);
        if ( params[name].getDagNode()->isParentInDAG( RbDagNodePtr( this ), done ) )
            throw RbException( "Invalid assignment: cycles in the DAG" );
    }

    /* Set parent(s) and add myself as a child to these */
    for ( size_t i = 0; i < params.size(); i++ ) {
        const std::string &name = params.getName(i);
        DAGNode* theParam = params[name].getDagNode();
        addParentNode( theParam );
        theParam->addChildNode(this);
    }

    /* We use a random draw as the initial value */
    value = distribution->rv().clone();
    if (value == NULL) {
        std::cerr << "Ooops, rv return NULL!\n";
    }
    
    /* Get initial probability */
    lnProb = calculateLnProbability();
    
    
    // decrement the reference count for myself
    RbDagNodePtr::decrementCountForAddress(this);
}


/** Copy constructor */
StochasticNode::StochasticNode( const StochasticNode& x ) : VariableNode( x ) {

    /* Set distribution */
    distribution = x.distribution->clone();

    /* Get distribution parameters */
    Environment& params = distribution->getMembers();

    /* Set parent(s) and add myself as a child to these */
    for ( size_t i = 0; i < params.size(); i++ ) {
        const std::string &name = params.getName(i);
        const RbDagNodePtr& theParam = params[name].getDagNode();
        addParentNode( theParam );
        theParam->addChildNode(this);
    }

    clamped             = x.clamped;
    instantiated        = x.instantiated;
    needsRecalculation  = x.needsRecalculation;
    value               = x.value->clone();
    touched             = x.touched;
    if ( x.touched == true ) {
        storedValue     = x.storedValue->clone();
    } else
        storedValue     = NULL;
    
    lnProb              = x.lnProb;
    storedLnProb        = x.storedLnProb;
}


/** Destructor */
StochasticNode::~StochasticNode( void ) {

    /* Remove parents first */
    for ( std::set<RbDagNodePtr >::iterator i = parents.begin(); i != parents.end(); i++ ) {
        const RbDagNodePtr& node = *i;
        node->removeChildNode( this );
    }
    parents.clear();
    
    delete distribution;
    
    delete value;
    
    if (storedValue != NULL) {
        delete storedValue;
    }
}


/** Assignment operator */
StochasticNode& StochasticNode::operator=( const StochasticNode& x ) {

    if ( this != &x ) {

        if ( valueTypeSpec != x.valueTypeSpec )
            throw RbException( "Type mismatch in StochasticNode assignment" );
        
        /* Remove parents first */
        for ( std::set<RbDagNodePtr >::iterator i = parents.begin(); i != parents.end(); i++ ) {
            RbDagNodePtr node = *i;
            node->removeChildNode( this );
        }
        parents.clear();

        /* Set distribution */
        distribution = x.distribution->clone();

        /* Get distribution parameters */
        Environment& params = distribution->getMembers();

        /* Set parent(s) and add myself as a child to these */
        for ( size_t i = 0; i < params.size(); i++ ) {
            const RbDagNodePtr& theParam = params[params.getName(i)].getVariable().getDagNode();
            addParentNode( theParam );
            theParam->addChildNode(this);
        }

        clamped             = x.clamped;
        instantiated        = x.instantiated;
        needsRecalculation  = x.needsRecalculation;
        
        value               = x.value->clone();
        touched             = x.touched;
        storedValue         = x.storedValue->clone();
        lnProb              = x.lnProb;
        storedLnProb        = x.storedLnProb;
        
        // set the name
        name = x.name;
    }

    return ( *this );
}


/** Are any distribution params touched? Get distribution params and check if any one is touched */
bool StochasticNode::areDistributionParamsTouched( void ) const {

    Environment& params = distribution->getMembers();

    for ( size_t i = 0; i < params.size(); i++ ) {
        
        const std::string &name = params.getName(i);
        const RbDagNodePtr& theNode  = params[name].getDagNode();

        if ( !theNode->isType( VariableNode_name ) )
            continue;

        if ( static_cast<const VariableNode*>( (DAGNode*)theNode )->isTouched() )
            return true;
    }

    return false;
}


/** Get the conditional ln probability of the node; do not rely on stored values */
double StochasticNode::calculateLnProbability( void ) {
    
    if (needsRecalculation) {
        if (instantiated) {
            lnProb = distribution->lnPdf( *value );
        }
        else {
            // we need to iterate over my states
            DistributionDiscrete* d = static_cast<DistributionDiscrete*>( distribution );
            d->getNumberOfStates();
        }
        
        needsRecalculation = false;
    }
    
    return lnProb;
}


/** Clamp the node to an observed value */
void StochasticNode::clamp( RbLanguageObject* observedVal ) {

    if ( touched )
        throw RbException( "Cannot clamp stochastic node in volatile state" );

    // touch for recalculation
    touch(); 
    
    // check for type conversion
    if (observedVal->isTypeSpec(distribution->getVariableType())) {
        if (value != NULL) {
            delete value;
        }
        value = observedVal;
        if (value == NULL) {
            std::cerr << "Ooops, observed value was NULL!\n";
        }
    }
    else if (observedVal->isConvertibleTo(distribution->getVariableType())) {
        value = static_cast<RbLanguageObject*>(observedVal->convertTo(distribution->getVariableType()) );
        if (value == NULL) {
            std::cerr << "Ooops, observed value was NULL!\n";
        }
        // we own the parameter so we need to delete it
        delete observedVal;
    }
    else {
        throw RbException("Cannot clamp stochastic node with value of type \"" + observedVal->getType() + "\" because the distribution requires a \"" + distribution->getVariableType().toString() + "\".");
    }

    clamped = true;
    lnProb  = calculateLnProbability();

}


/** Clone this object */
StochasticNode* StochasticNode::clone( void ) const {

    return new StochasticNode( *this );
}


/** Clone the entire graph: clone children, swap parents */
RbDagNodePtr StochasticNode::cloneDAG( std::map<const DAGNode*, RbDagNodePtr >& newNodes ) const {

    if ( newNodes.find( this ) != newNodes.end() )
        return ( newNodes[ this ] );

    /* Get pristine copy */
    StochasticNode* copy = new StochasticNode( valueTypeSpec );
    newNodes[ this ] = copy;
    
    /* Set the name so that the new node remains identifiable */
    copy->setName(name);

    /* Set the copy member variables */
    copy->distribution = distribution->clone();
    copy->clamped      = clamped;
    copy->touched      = touched;
    if (value != NULL) {
        copy->value    = value->clone();
    }
    if (storedValue == NULL)
        copy->storedValue = NULL;
    else {
        copy->storedValue = storedValue->clone();
    }
    copy->lnProb             = lnProb;
    copy->storedLnProb       = storedLnProb;
    copy->needsRecalculation = needsRecalculation;

    /* Set the copy params to their matches in the new DAG */
    Environment& params     = distribution->getMembers();
    Environment& copyParams = copy->distribution->getMembers();

    for ( size_t i = 0; i < params.size(); i++ ) {

        // get the name if the i-th member
        const std::string &name = params.getName(i);
        
        // clone the member and get the clone back
        const RbDagNodePtr& theParam = params[name].getDagNode();
        // if we already have cloned this parent (parameter), then we will get the previously created clone
        RbDagNodePtr theParamClone = theParam->cloneDAG( newNodes );
        
        // set the clone of the member as the member of the clone
        copyParams[name].setVariable( new Variable(theParamClone) );

        copy->addParentNode( theParamClone );
        theParamClone->addChildNode( copy );
    }

    /* Make sure the children clone themselves */
    for( std::set<VariableNode*>::const_iterator i = children.begin(); i != children.end(); i++ ) {
        (*i)->cloneDAG( newNodes );
    }

    return copy;
}


/** Complete info about object */
std::string StochasticNode::debugInfo(void) const {
    
    std::ostringstream o;
    o << "StochasticNode:" << std::endl;
    o << "Clamped      = " << ( clamped ? "true" : "false" ) << std::endl;
    o << "Touched      = " << ( touched ? "true" : "false" ) << std::endl;
    o << "Distribution = ";
    distribution->printValue( o );
    o << std::endl;
    o << "Value        = ";
    value->printValue( o );
    o << std::endl;
    o << "Stored value = ";
    if ( storedValue == NULL )
        o << "NULL";
    else
        storedValue->printValue( o );
    
    return o.str();
}


/** Get class vector describing type of DAG node */
const VectorString& StochasticNode::getClass() const {

    static VectorString rbClass = VectorString( StochasticNode_name ) + VariableNode::getClass();
    return rbClass;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& StochasticNode::getTypeSpec(void) const {
    return typeSpec;
}


/** Get affected nodes: RbDagNodePtr this node and only stop recursion here if instantiated, otherwise (if integrated over) we pass on the recursion to our children */
void StochasticNode::getAffected( std::set<StochasticNode* >& affected ) {
    
    /* If we have already touched this node, we are done; otherwise, get the affected children */
    //    if ( !touched ) {
    //        touched = true;
    affected.insert( this );
    
    // if this node is integrated out, then we need to add the children too
    if (!instantiated) {
        for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ ) {
            (*i)->getAffected( affected );
        }
    }
    //    }
}


const Distribution& StochasticNode::getDistribution(void) const {
    return *distribution;
}


Distribution& StochasticNode::getDistribution(void) {
    return *distribution;
}


/** Get the ln probability ratio of this node */
double StochasticNode::getLnProbabilityRatio( void ) {
    
    if ( !isTouched() ) {

        return 0.0;
    }
    else {
//        assert( !areDistributionParamsTouched() );
        
        return calculateLnProbability() - storedLnProb;
    }
}


/** Get stored value */
const RbLanguageObject& StochasticNode::getStoredValue( void ) const {

    if ( !touched )
        return *value;

    return *storedValue;
}


/** Get const value; we always know our value. */
const RbLanguageObject& StochasticNode::getValue( void ) const {

    return *value;
}

/** Get non-const value; we always know our value. */
RbLanguageObject& StochasticNode::getValue( void ) {
    
    return *value;
}


///** Get value pointer; we always know our value. */
//const RbLanguageObject* StochasticNode::getValuePtr( void ) const {
//    
//    return value;
//}


/**
 * Keep the current value of the node. 
 * At this point, we also need to make sure we update the stored ln probability.
 */
void StochasticNode::keepMe() {

    if ( touched ) {
        
        // delete the stored value
        if (storedValue != NULL) {
            delete storedValue;
        }
        storedValue = NULL;
        
        storedLnProb = 1.0E6;       // An almost impossible value for the density
        if (needsRecalculation) {
            lnProb = calculateLnProbability();
        }
    }

    touched = false;

}


/** Print struct for user */
void StochasticNode::printStruct( std::ostream& o ) const {

    o << "_Class        = " << getClass() << std::endl;
    o << "_Adress       = " << this << std::endl;
    o << "_valueType    = " << getValueType() << std::endl;
    o << "_distribution = ";
    distribution->printValue(o);
    o << std::endl;
    o << "_touched      = " << ( touched ? RbBoolean( true ) : RbBoolean( false ) ) << std::endl;
    o << "_clamped      = " << ( clamped ? RbBoolean( true ) : RbBoolean( false ) ) << std::endl;
    o << "_value        = ";
    value->printValue(o);
    o << std::endl;
    if ( touched ) {
        o << "_storedValue  = "; 
        storedValue->printValue(o);
        o << std::endl;
    }
    o << "_lnProb       = " << lnProb << std::endl;
    if ( touched )
        o << "_storedLnProb = " << storedLnProb << std::endl;    

    o << "_parents      = ";
    printParents(o);
    o << std::endl;

    o << "_children     = ";
    printChildren(o);
    o << std::endl;

    o << std::endl;
}


/** Print value for user */
void StochasticNode::printValue( std::ostream& o ) const {

    if ( touched )
        RBOUT( "Warning: Variable in touched state" );

    if (value == NULL) {
        o << "NULL";
    }
    else {
        value->printValue(o);
    }
}


/** Restore the old value of the node and tell affected */
void StochasticNode::restoreMe() {

    if ( touched ) {
        
        if (!clamped) {
            if (storedValue != NULL) {
                if (value != NULL) {
                    delete value;
                }
                value           = storedValue;
            
                // delete the stored value
                storedValue = NULL;
            }
        }
        
        lnProb          = storedLnProb;
        storedLnProb    = 1.0E6;    // An almost impossible value for the density
        
        needsRecalculation = false;
    }

    touched = false;
}


/**
 * Set value: same as clamp, but do not clamp. This function will
 * also be used by moves to propose a new value.
 */
void StochasticNode::setValue( RbLanguageObject* val ) {

    if (val == NULL) {
        std::cerr << "Ooops ..." << std::endl;
    }
    
    if ( clamped )
        throw RbException( "Cannot change value of clamped node" );

    // touch the node (which will store the lnProb)
    touch();
    
    // delete the stored value
    if (storedValue == NULL) {
        storedValue = value;
    }
    else {
        delete value;
    }
    

    // set the value
    value = val;
    if (value == NULL) {
        std::cerr << "Ooops, setting value to NULL!\n";
    }
}


/**
 * Swap parent node. Note that this function will be called by the tree plate
 * when the topology is changed. It is also used by the parser.
 * This function should be called from the mutateTo function, otherwise it
 * is dangerous because the distribution parameters will not be accommodated.
 */
void StochasticNode::swapParentNode(const RbDagNodePtr& oldNode, const RbDagNodePtr& newNode ) {

    if ( parents.find( oldNode ) == parents.end() )
        throw RbException( "Node is not a parent" );

    oldNode->removeChildNode( this );
    newNode->addChildNode   ( this );
    removeParentNode( oldNode );
    addParentNode( newNode );

    if ( clamped == false ) {

        touchMe();
        
        // We keep the current value; delegate moves can later change the value
        // if they are interested.
    }

    //for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ )
    //    (*i)->touch();
}


/** touch this node for recalculation */
void StochasticNode::touchMe( void ) {
    if (!touched) {
        // Store the current lnProb 
        touched      = true;
    
        storedLnProb = lnProb;
    }
    
    needsRecalculation = true;
}

/** Unclamp the value; use the clamped value as initial value */
void StochasticNode::unclamp( void ) {

    clamped = false;
}

