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

#include "Boolean.h"
#include "Distribution.h"
#include "MoveSchedule.h"
#include "RbException.h"
#include "RbNames.h"
#include "StochasticNode.h"
#include "UserInterface.h"
#include "VectorString.h"
#include "Workspace.h"

#include <algorithm>
#include <cassert>


/** Constructor of empty StochasticNode */
StochasticNode::StochasticNode( const TypeSpec& typeSp )
    : VariableNode( typeSp.getType() ), valueDim( typeSp.getDim() ), clamped( false ), distribution( NULL ) {
}


/** Constructor from distribution */
StochasticNode::StochasticNode( Distribution* dist )
    : VariableNode( dist->getVariableType().getType() ), valueDim( dist->getVariableType().getDim() ), clamped( false ), distribution( dist ) {

    /* Get distribution parameters */
    const MemberFrame& params = dist->getMembers();

    /* Check for cycles */
    std::list<DAGNode*> done;
    for ( size_t i = 0; i < params.size(); i++ ) {
        done.clear();
        if ( params[i].getVariable()->isParentInDAG( this, done ) )
            throw RbException( "Invalid assignment: cycles in the DAG" );
    }

    /* Set parent(s) and add myself as a child to these */
    for ( size_t i = 0; i < params.size(); i++ ) {
        DAGNode* theParam = const_cast<DAGNode*>( params[i].getVariable() );
        parents.insert( theParam );
        theParam->addChildNode(this);
    }

    /* We use a random draw as the initial value */
    value = distribution->rv();

    /* Get initial probability */
    lnProb = calculateLnProbability();
}


/** Copy constructor */
StochasticNode::StochasticNode( const StochasticNode& x ) : VariableNode( x ) {

    /* Set distribution */
    distribution = x.distribution->clone();

    /* Get distribution parameters */
    const MemberFrame& params = distribution->getMembers();

    /* Set parent(s) and add myself as a child to these */
    for ( size_t i = 0; i < params.size(); i++ ) {
        DAGNode* theParam = const_cast<DAGNode*>( params[i].getVariable() );
        parents.insert( theParam );
        theParam->addChildNode(this);
    }

    valueDim     = x.valueDim;
    clamped      = x.clamped;
    value        = x.value->clone();
    touched      = x.touched;
    if ( x.touched == true )
        storedValue  = x.storedValue->clone();
    else
        storedValue = NULL;
    lnProb       = x.lnProb;
    storedLnProb = x.storedLnProb;
}


/** Destructor */
StochasticNode::~StochasticNode( void ) {

    if ( numRefs() != 0 )
        throw RbException ( "Cannot delete StochasticNode with references" ); 

    /* Remove parents first */
    for ( std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); i++ )
        (*i)->removeChildNode( this );
    parents.clear();

    delete distribution;    // This will delete any DAG nodes that need to be deleted
}


/** Assignment operator */
StochasticNode& StochasticNode::operator=( const StochasticNode& x ) {

    if ( this != &x ) {

        if ( valueType != x.valueType || valueDim != x.valueDim )
            throw RbException( "Type mismatch in StochasticNode assignment" );
        
        /* Remove parents first */
        for ( std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); i++ )
            (*i)->removeChildNode( this );
        parents.clear();

        delete distribution;    // This will delete any DAG nodes that need to be deleted

        /* Set distribution */
        distribution = x.distribution->clone();

        /* Get distribution parameters */
        const MemberFrame& params = distribution->getMembers();

        /* Set parent(s) and add myself as a child to these */
        for ( size_t i = 0; i < params.size(); i++ ) {
            DAGNode* theParam = const_cast<DAGNode*>( params[i].getVariable() );
            parents.insert( theParam );
            theParam->addChildNode(this);
        }

        clamped      = x.clamped;
        value        = x.value->clone();
        touched      = x.touched;
        if ( x.touched == true )
            storedValue  = x.storedValue->clone();
        else
            storedValue = NULL;
        lnProb       = x.lnProb;
        storedLnProb = x.storedLnProb;
    }

    return ( *this );
}


/** Are any distribution params touched? Get distribution params and check if any one is touched */
bool StochasticNode::areDistributionParamsTouched( void ) const {

    const MemberFrame& params = distribution->getMembers();

    for ( size_t i = 0; i < params.size(); i++ ) {

        if ( !params[i].getVariable()->isDAGType( VariableNode_name ) )
            continue;

        if ( static_cast<const VariableNode*>( params[i].getVariable() )->isTouched() )
            return true;
    }

    return false;
}


/** Clamp the node to an observed value */
void StochasticNode::clamp( RbObject* observedVal ) {

    if ( touched )
        throw RbException( "Cannot clamp stochastic node in volatile state" );

    delete value;
    value   = observedVal;
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
    StochasticNode* copy = new StochasticNode( TypeSpec( valueType, valueDim ) );
    newNodes[ this ] = copy;

    /* Set the copy member variables */
    copy->distribution = distribution->clone();
    copy->clamped      = clamped;
    copy->touched      = touched;
    copy->value        = value->clone();
    if (storedValue == NULL)
        copy->storedValue = NULL;
    else
        copy->storedValue = storedValue->clone();
    copy->lnProb       = lnProb;
    copy->storedLnProb = storedLnProb;

    /* Set the copy params to their matches in the new DAG */
    const MemberFrame& params     = distribution->getMembers();
    MemberFrame&       copyParams = const_cast<MemberFrame&>( copy->distribution->getMembers() );

    for ( size_t i = 0; i < params.size(); i++ ) {

        DAGNode* theParamClone = params[i].getVariable()->cloneDAG( newNodes );
        copyParams[i].replaceVariable( theParamClone );

        copy->parents.insert( theParamClone );
        theParamClone->addChildNode( copy );
    }

    /* Make sure the children clone themselves */
    for( std::set<VariableNode*>::const_iterator i = children.begin(); i != children.end(); i++ ) {
        (*i)->cloneDAG( newNodes );
    }

    return copy;
}


/** Get affected nodes: insert this node but stop recursion here unless touched */
void StochasticNode::getAffected( std::set<StochasticNode*>& affected ) {

    if ( touched ) {
    
        for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ )
            (*i)->getAffected(affected);
    }
    else {

        affected.insert( this );
    }
}


/** Get class vector describing type of DAG node */
const VectorString& StochasticNode::getDAGClass() const {

    static VectorString rbClass = VectorString( StochasticNode_name ) + VariableNode::getDAGClass();
    return rbClass;
}


/** Get default moves from distribution */
MoveSchedule* StochasticNode::getDefaultMoves( void ) {

    MoveSchedule* defaultMoves = new MoveSchedule( this, 1.0 );
    defaultMoves->addMove( distribution->getDefaultMove(this) );

    return defaultMoves;
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
const RbObject* StochasticNode::getStoredValue( void ) {

    if ( !touched )
        return value;

    return storedValue;
}


/** Get const value; we always know our value. */
const RbObject* StochasticNode::getValue( void ) const {

    return value;
}


/**
 * Keep the current value of the node and tell affected. At this point,
 * we also need to make sure we update the stored ln probability.
 */
void StochasticNode::keep() {

    if ( touched ) {

        delete storedValue;
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

    o << "_DAGClass     = " << getDAGClass() << std::endl;
    o << "_valueType    = " << valueType << std::endl;
    o << "_dim          = " << getDim() << std::endl;
    o << "_distribution = " << distribution->briefInfo() << std::endl;
    o << "_touched      = " << ( touched ? Boolean( true ) : Boolean( false ) ) << std::endl;
    o << "_clamped      = " << ( clamped ? Boolean( true ) : Boolean( false ) ) << std::endl;
    o << "_value        = " << value->briefInfo() << std::endl;
    if ( touched )
        o << "_storedValue  = " << storedValue->briefInfo() << std::endl;
    o << "_lnProb       = " << value->briefInfo() << std::endl;
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
void StochasticNode::printValue( std::ostream& o ) const {

    if ( touched )
        RBOUT( "Warning: Variable in volatile state so value is unreliable" );

    value->printValue(o);
}


/** Restore the old value of the node and tell affected */
void StochasticNode::restore() {

    if ( touched ) {

        delete value;
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
void StochasticNode::setValue( RbObject* val ) {

    if ( clamped )
        throw RbException( "Cannot change value of clamped node" );

    if ( !touched ) {

        // Store the current value and replace with val
        touched      = true;
        storedValue  = value;
        storedLnProb = lnProb;
        value        = val;
    }
    else /* if ( touched ) */ {

        // Keep the old storedValue and storedLnProb but delete the current value
        delete value;
        value        = val;
    }

    for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->touchAffected();
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
    parents.erase ( oldNode );
    parents.insert( newNode );

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

