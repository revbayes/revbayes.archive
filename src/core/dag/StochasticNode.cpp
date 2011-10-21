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
#include "MoveSchedule.h"
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
StochasticNode::StochasticNode( const TypeSpec& typeSp ) : VariableNode( typeSp.getType() ), clamped( false ), distribution( NULL ) {
}


/** Constructor from distribution */
StochasticNode::StochasticNode( Distribution* dist ) : VariableNode( dist->getVariableType().getType() ), clamped( false ), distribution( dist ) {
    
    // retain the distribution
    distribution->retain();

    /* Get distribution parameters */
    const MemberEnvironment& params = dist->getMembers();

    /* Check for cycles */
    std::list<DAGNode*> done;
    for ( size_t i = 0; i < params.size(); i++ ) {
        done.clear();
        const std::string &name = params.getName(i);
        if ( params[name].getDagNode()->isParentInDAG( this, done ) )
            throw RbException( "Invalid assignment: cycles in the DAG" );
    }

    /* Set parent(s) and add myself as a child to these */
    for ( size_t i = 0; i < params.size(); i++ ) {
        const std::string &name = params.getName(i);
        DAGNode* theParam = const_cast<DAGNode*>( params[name].getDagNode() );
        addParentNode( theParam );
        theParam->addChildNode(this);
    }

    /* We use a random draw as the initial value */
    value = distribution->rv();
    value->retain();

    /* Get initial probability */
    lnProb = calculateLnProbability();
}


/** Copy constructor */
StochasticNode::StochasticNode( const StochasticNode& x ) : VariableNode( x ) {

    /* Set distribution */
    distribution = x.distribution->clone();
    distribution->retain();

    /* Get distribution parameters */
    const MemberEnvironment& params = distribution->getMembers();

    /* Set parent(s) and add myself as a child to these */
    for ( size_t i = 0; i < params.size(); i++ ) {
        const std::string &name = params.getName(i);
        DAGNode* theParam = const_cast<DAGNode*>( params[name].getDagNode() );
        addParentNode( theParam );
        theParam->addChildNode(this);
    }

    clamped      = x.clamped;
    value        = x.value->clone();
    value->retain();
    touched      = x.touched;
    if ( x.touched == true ) {
        storedValue  = x.storedValue->clone();
        storedValue->retain();
    } else
        storedValue = NULL;
    lnProb       = x.lnProb;
    storedLnProb = x.storedLnProb;
}


/** Destructor */
StochasticNode::~StochasticNode( void ) {

    /* Remove parents first */
    for ( std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); i++ )
        (*i)->removeChildNode( this );
    parents.clear();

    if (distribution != NULL) {
        distribution->release();
        if (distribution->isUnreferenced()) {
            delete distribution;    // This will delete any DAG nodes that need to be deleted
        }
    }
}


/** Assignment operator */
StochasticNode& StochasticNode::operator=( const StochasticNode& x ) {

    if ( this != &x ) {

        if ( valueTypeSpec != x.valueTypeSpec )
            throw RbException( "Type mismatch in StochasticNode assignment" );
        
        /* Remove parents first */
        for ( std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); i++ ) {
            (*i)->removeChildNode( this );
            (*i)->release();
            if ((*i)->isUnreferenced()){
                delete (*i);
            }
        }
        parents.clear();

        if (distribution != NULL) {
            distribution->release();
            if (distribution->isUnreferenced()) {
                
                delete distribution;    // This will delete any DAG nodes that need to be deleted
            }
        }

        /* Set distribution */
        distribution = x.distribution->clone();
        distribution->retain();

        /* Get distribution parameters */
        const MemberEnvironment& params = distribution->getMembers();

        /* Set parent(s) and add myself as a child to these */
        for ( size_t i = 0; i < params.size(); i++ ) {
            DAGNode* theParam = params[params.getName(i)].getVariable()->getDagNodePtr();
            addParentNode( theParam );
            theParam->addChildNode(this);
        }

        clamped      = x.clamped;
        
        // release the old value
        if (value != NULL) {
            value->release();
            if (value->isUnreferenced()) {
                delete value;
            }
        }
        value        = x.value->clone();
        value->retain();
        touched      = x.touched;
        if ( x.touched == true )
            storedValue  = x.storedValue->clone();
        else
            storedValue = NULL;
        lnProb       = x.lnProb;
        storedLnProb = x.storedLnProb;
        
        // set the name
        name = x.name;
    }

    return ( *this );
}


/** Are any distribution params touched? Get distribution params and check if any one is touched */
bool StochasticNode::areDistributionParamsTouched( void ) const {

    const MemberEnvironment& params = distribution->getMembers();

    for ( size_t i = 0; i < params.size(); i++ ) {
        
        const std::string &name = params.getName(i);
        const DAGNode *theNode  = params[name].getDagNode();

        if ( !theNode->isType( VariableNode_name ) )
            continue;

        if ( static_cast<const VariableNode*>( theNode )->isTouched() )
            return true;
    }

    return false;
}


/** Clamp the node to an observed value */
void StochasticNode::clamp( RbLanguageObject* observedVal ) {

    if ( touched )
        throw RbException( "Cannot clamp stochastic node in volatile state" );

    if (value != NULL) {
        value->release();
        if(value->isUnreferenced()) {
            delete value;
        }
    }
    
    // check for type conversion
    if (observedVal->isTypeSpec(distribution->getVariableType())) {
        value = observedVal;
    }
    else if (observedVal->isConvertibleTo(distribution->getVariableType())) {
        value = static_cast<RbLanguageObject*>(observedVal->convertTo(distribution->getVariableType())); 
    }
    else {
        throw RbException("Cannot clamp stochastic node with value of type \"" + observedVal->getType() + "\" because the distribution requires a \"" + distribution->getVariableType().toString() + "\".");
    }

    value->retain();
    clamped = true;
    lnProb  = calculateLnProbability();

    for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++)
        (*i)->touchAffected();
}


/** Clone this object */
StochasticNode* StochasticNode::clone( void ) const {

    return new StochasticNode( *this );
}


/** Clone the entire graph: clone children, swap parents */
StochasticNode* StochasticNode::cloneDAG( std::map<const DAGNode*, DAGNode*>& newNodes ) const {

    if ( newNodes.find( this ) != newNodes.end() )
        return static_cast<StochasticNode*>( newNodes[ this ] );

    /* Get pristine copy */
    StochasticNode* copy = new StochasticNode( valueTypeSpec );
    newNodes[ this ] = copy;
    
    /* Set the name so that the new node remains identifiable */
    copy->setName(name);

    /* Set the copy member variables */
    copy->distribution = distribution->clone();
    copy->distribution->retain();
    copy->clamped      = clamped;
    copy->touched      = touched;
    if (value != NULL) {
        copy->value        = value->clone();
        copy->value->retain();
    }
    if (storedValue == NULL)
        copy->storedValue = NULL;
    else {
        copy->storedValue = storedValue->clone();
        copy->storedValue->retain();
    }
    copy->lnProb       = lnProb;
    copy->storedLnProb = storedLnProb;

    /* Set the copy params to their matches in the new DAG */
    const MemberEnvironment& params     = distribution->getMembers();
    MemberEnvironment&       copyParams = const_cast<MemberEnvironment&>( copy->distribution->getMembers() );

    for ( size_t i = 0; i < params.size(); i++ ) {

        // get the name if the i-th member
        const std::string &name = params.getName(i);
        
        // clone the member and get the clone back
        const DAGNode* theParam = params[name].getDagNode();
        DAGNode* theParamClone  = theParam->cloneDAG( newNodes );
        
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


/** Get affected nodes: insert this node but stop recursion here */
void StochasticNode::getAffected( std::set<StochasticNode*>& affected ) {

    affected.insert( this );
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


/** Get the conditional ln probability of the node; do not rely on stored values */
double StochasticNode::calculateLnProbability( void ) {

	return distribution->lnPdf( value );
}


/** Get the ln probability ratio of this node */
double StochasticNode::getLnProbabilityRatio( void ) {

    if ( !isTouched() && !areDistributionParamsTouched() ) {

        return 0.0;
    }
    else if ( isTouched() && !areDistributionParamsTouched() ) {

        return distribution->lnPdf( value ) - storedLnProb;
    }
    else if ( !isTouched() && areDistributionParamsTouched() ) {

        return distribution->lnPdf( value) - lnProb;
    }
    else /* if ( isTouched() && areDistributionParamsTouched() ) */ {

        return distribution->lnPdf( value ) - storedLnProb;
    }
}


/** Get stored value */
const RbLanguageObject* StochasticNode::getStoredValue( void ) {

    if ( !touched )
        return value;

    return storedValue;
}


/** Get const value; we always know our value. */
const RbLanguageObject* StochasticNode::getValue( void ) {

    return value;
}

/** Get const value; we always know our value. */
RbLanguageObject* StochasticNode::getValuePtr( void ) {
    
    return value;
}


/**
 * Keep the current value of the node and tell affected. At this point,
 * we also need to make sure we update the stored ln probability.
 */
void StochasticNode::keep() {

    if ( touched ) {

        if (storedValue != NULL) {
            storedValue->release();
            if (storedValue->isUnreferenced()) {
                delete storedValue;
            }
        }
        storedValue = NULL;
        storedLnProb = 1.0E6;       // An almost impossible value for the density
        lnProb = calculateLnProbability();
    }

    touched = false;

    for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->keepAffected();
}


/**
 * Tell affected variable nodes to keep the current value: stop the recursion here.
 * We also need to update our probability.
 */
void StochasticNode::keepAffected() {

    lnProb = calculateLnProbability();
}


/** Print struct for user */
void StochasticNode::printStruct( std::ostream& o ) const {

    o << "_Class        = " << getClass() << std::endl;
    o << "_valueType    = " << getValueType() << std::endl;
    o << "_distribution = " << distribution->briefInfo() << std::endl;
    o << "_touched      = " << ( touched ? RbBoolean( true ) : RbBoolean( false ) ) << std::endl;
    o << "_clamped      = " << ( clamped ? RbBoolean( true ) : RbBoolean( false ) ) << std::endl;
    o << "_value        = " << value->briefInfo() << std::endl;
    if ( touched )
        o << "_storedValue  = " << storedValue->briefInfo() << std::endl;
    o << "_lnProb       = " << lnProb << std::endl;
    if ( touched )
        o << "_storedLnProb = " << storedValue->briefInfo() << std::endl;    

    o << "_parents      = ";
    printParents(o);
    o << std::endl;

    o << "_children     = ";
    printChildren(o);
    o << std::endl;

    o << std::endl;
}


/** Print value for user */
void StochasticNode::printValue( std::ostream& o ) {

    if ( touched )
        RBOUT( "Warning: Variable in touched state" );

    value->printValue(o);
}


/** Restore the old value of the node and tell affected */
void StochasticNode::restore() {

    if ( touched ) {
        
        if (value != NULL) {
            value->release();
            if(value->isUnreferenced()) {
                delete value;
            }
        }
        value           = storedValue;
        storedValue     = NULL;
        lnProb          = storedLnProb;
        storedLnProb    = 1.0E6;    // An almost impossible value for the density
    }

    touched = false;
 
    for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->restoreAffected();
}


/** Tell affected variable nodes to restore the old value: stop the recursion here */
void StochasticNode::restoreAffected() {
}


/** Complete info about object */
std::string StochasticNode::richInfo(void) const {

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


/**
 * Set value: same as clamp, but do not clamp. This function will
 * also be used by moves to propose a new value.
 */
void StochasticNode::setValue( RbLanguageObject* val, std::set<StochasticNode*>& affected ) {

    if ( clamped )
        throw RbException( "Cannot change value of clamped node" );

    if ( !touched ) {

        // Store the current value and replace with val
        touched      = true;
        
        // release old stored value
        if (storedValue != NULL) {
            storedValue->release();
            if(storedValue->isUnreferenced()) {
                delete storedValue;
            }
        }
        storedValue  = value;
        storedLnProb = lnProb;
        value        = val;
        
        // retain the new stored value
        value->retain();
    }
    else /* if ( touched ) */ {

        // Keep the old storedValue and storedLnProb but delete the current value
        if (value != NULL) {
            value->release();
            if(value->isUnreferenced()) {
                delete value;
            }
        }
        value        = val;
        value->retain();
    }

    for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->getAffected( affected);
}


/**
 * Swap parent node. Note that this function will be called by the tree plate
 * when the topology is changed. It is also used by the parser.
 * This function should be called from the mutateTo function, otherwise it
 * is dangerous because the distribution parameters will not be accommodated.
 */
void StochasticNode::swapParentNode( DAGNode* oldNode, DAGNode* newNode ) {

    if ( parents.find( oldNode ) == parents.end() )
        throw RbException( "Node is not a parent" );

    oldNode->removeChildNode( this );
    newNode->addChildNode   ( this );
    removeParentNode( oldNode );
    addParentNode( newNode );

    if ( clamped == false ) {

        if ( !touched ) {
            storedValue = value->clone();
            storedLnProb = lnProb;
            touched = true;
        }

        // We keep the current value; delegate moves can later change the value
        // if they are interested.
    }

    for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->touchAffected();
}


/** Unclamp the value; use the clamped value as initial value */
void StochasticNode::unclamp( void ) {

    clamped = false;
}

