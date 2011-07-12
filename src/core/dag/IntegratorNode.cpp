/**
 * @file
 * This file contains the implementation of IntegratorNode, which is derived
 * from StochasticNode. IntegratorNode is used for DAG nodes integrating
 * out probabilities over a conditional likelihood vector, given the probability
 * mass vector from an appropriate categorical distribution.
 *
 * @brief Implementation of IntegratorNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2010-01-12 21:51:54 +0100 (Tis, 12 Jan 2010) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: IntegratorNode.cpp 221 2010-01-12 20:51:54Z Hoehna $
 */

#include "Boolean.h"
#include "DistributionDiscrete.h"
#include "IntegratorNode.h"
#include "MemberNode.h"
#include "MoveSchedule.h"
#include "RbException.h"
#include "RbNames.h"
#include "Simplex.h"
#include "UserInterface.h"
#include "CondLike.h"
#include "VectorString.h"
#include "Workspace.h"

#include <algorithm>
#include <cassert>


/** Constructor of empty IntegratorNode */
IntegratorNode::IntegratorNode( void )
    : StochasticNode( RbVoid_name ), condLikes( NULL ) {
}


/** Constructor from cond likes and distribution */
IntegratorNode::IntegratorNode( MemberNode* condLikeVec, Distribution* dist )
    : StochasticNode( RbVoid_name ), condLikes( condLikeVec ) {

    /* Set distribution here so we avoid inappropriate type for IntegratorNode. Also check type. For now, we only
       allow distributions on discrete values but we should allow continuous distributions in the future. */
    if ( !dist->isType( DistributionDiscrete_name ) )
        throw RbException( "Invalid attempt to create IntegratorNode: distribution type is incorrect" );
    distribution = dist;

    /* Get distribution parameters */
    const MemberFrame& params = dist->getMembers();

    /* Check for cycles */
    std::list<DAGNode*> done;
    for ( size_t i = 0; i < params.size(); i++ ) {
        done.clear();
        if ( params[i].getVariable()->isParentInDAG( this, done ) )
            throw RbException( "Invalid assignment: cycles in the DAG" );
    }
    done.clear();
    if ( condLikes->isParentInDAG( this, done ) )
        throw RbException( "Invalid assignment: cycles in the DAG" );

    /* Set parent(s) and add myself as a child to these */
    for ( size_t i = 0; i < params.size(); i++ ) {
        DAGNode* theParam = const_cast<DAGNode*>( params[i].getVariable() );
        parents.insert( theParam );
        theParam->addChildNode( this );
    }
    parents.insert( condLikes );
    condLikes->addChildNode( this );

    /* Get initial probability */
    lnProb = calculateLnProbability();
}


/** Copy constructor */
IntegratorNode::IntegratorNode( const IntegratorNode& x ) : StochasticNode( x ) {

    /* Set cond likes */
    condLikes    = x.condLikes->clone();
    parents.insert( condLikes );
    condLikes->addChildNode( this );
}


/** Destructor */
IntegratorNode::~IntegratorNode( void ) {

    if ( numRefs() != 0 )
        throw RbException ( "Cannot delete IntegratorNode with references" ); 

    /* Remove parents first so that parent destructor does not get in the way */
    for ( std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); i++ )
        (*i)->removeChildNode( this );
    parents.clear();

    if ( condLikes->numRefs() == 0 )
        delete condLikes;

    delete distribution;    // This will delete any remainin DAG nodes that need to be deleted
}


/** Assignment operator */
IntegratorNode& IntegratorNode::operator=( const IntegratorNode& x ) {

    if ( this != &x ) {

        if ( valueType != x.valueType )
            throw RbException( "Type mismatch in IntegratorNode assignment" );
        
        /* Remove parents first */
        for ( std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); i++ )
            (*i)->removeChildNode( this );
        parents.clear();

        if ( condLikes->numRefs() == 0 )
            delete condLikes;

        delete distribution;    // This will delete any remaining DAG nodes that need to be deleted

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


/**
 * Calculate the conditional ln probability of the node.
 * For now, we do not support scaling or character compression.
 */
double IntegratorNode::calculateLnProbability( void ) {

    // Get all the values we need
    const CondLike* condLikePtr = static_cast<const CondLike*>( condLikes->getValue() );
    size_t          numChars    = condLikePtr->getNumChars();
    size_t          numStates   = condLikePtr->getNumStates();
    const double*   cL          = &(*condLikePtr)[0];
    const Simplex*  m           = static_cast<DistributionDiscrete*>( distribution )->getProbabilityMassVector();

    // Initialize C array with pi values (I suspect this is faster than using the simplex directly)
    double*                 pi          = new double[ numStates ];
    for ( size_t i = 0; i < numStates; i++ )
        pi[i] = (*m)[i];

    // Now calculate the ln probability
    double lnProb = 0.0;
    for ( size_t i = 0; i < numChars; i++ ) {
        
        double charCondLike = 0.0;
        for ( size_t j = 0; j < numStates; j++, cL++ )
            charCondLike += (*cL) * pi[j];                

        lnProb += charCondLike;
    }

    delete[] pi;

    return lnProb;
}


/** Clamp the node to an observed value */
void IntegratorNode::clamp( RbObject* observedVal ) {

    throw RbException( "Cannot clamp integrator node" );
}


/** Clone this object */
IntegratorNode* IntegratorNode::clone( void ) const {

    return new IntegratorNode( *this );
}


/** Clone the entire graph: clone children, swap parents */
IntegratorNode* IntegratorNode::cloneDAG( std::map<const DAGNode*, DAGNode*>& newNodes ) const {

    if ( newNodes.find( this ) != newNodes.end() )
        return static_cast<IntegratorNode*>( newNodes[ this ] );

    /* Get pristine copy */
    IntegratorNode* copy = new IntegratorNode();
    newNodes[ this ] = copy;

    /* Set the copy member variables */
    copy->distribution = distribution->clone();
    copy->clamped      = clamped;
    copy->touched      = touched;
    copy->value        = NULL;
    copy->storedValue  = NULL;
    copy->lnProb       = lnProb;
    copy->storedLnProb = storedLnProb;

    /* Set the copy cond likes node to its match in the new DAG */
    MemberNode* theCondLikesClone = condLikes->cloneDAG( newNodes );
    copy->condLikes = theCondLikesClone;
    copy->parents.insert( theCondLikesClone );
    theCondLikesClone->addChildNode( copy );

    /* Set the copy distribution params to their matches in the new DAG */
    const MemberFrame& params     = distribution->getMembers();
    MemberFrame&       copyParams = const_cast<MemberFrame&>( copy->distribution->getMembers() );

    for ( size_t i = 0; i < params.size(); i++ ) {

        DAGNode* theParamClone = params[i].getVariable()->cloneDAG( newNodes );
        copyParams[i].replaceDAGVariable( theParamClone );

        copy->parents.insert( theParamClone );
        theParamClone->addChildNode( copy );
    }

    /* Make sure the children clone themselves */
    for( std::set<VariableNode*>::const_iterator i = children.begin(); i != children.end(); i++ ) {
        (*i)->cloneDAG( newNodes );
    }

    return copy;
}


/** Get class vector describing type of DAG node */
const VectorString& IntegratorNode::getDAGClass() const {

    static VectorString rbClass = VectorString( IntegratorNode_name ) + VariableNode::getDAGClass();
    return rbClass;
}


/** Get default moves from distribution */
MoveSchedule* IntegratorNode::getDefaultMoves( void ) {

    throw RbException( "IntegratorNode does not have moves" );
}


/** Get the ln probability ratio of this node */
double IntegratorNode::getLnProbabilityRatio( void ) {

    if ( !isTouched() && !areDistributionParamsTouched() ) {

        return 0.0;
    }
    else if ( isTouched() && !areDistributionParamsTouched() ) {

        return calculateLnProbability() - storedLnProb;
    }
    else if ( !isTouched() && areDistributionParamsTouched() ) {

        return calculateLnProbability() - lnProb;
    }
    else /* if ( isTouched() && areParentsTouched() ) */ {

        return calculateLnProbability() - storedLnProb;
    }
}


/** Keep the current value of the node and tell affected */
void IntegratorNode::keep() {

    if ( touched ) {

        storedLnProb = 1.0E6;       // An almost impossible value for the density
        lnProb = calculateLnProbability();
    }

    touched = false;

    for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->keepAffected();
}


/** Print struct for user */
void IntegratorNode::printStruct( std::ostream& o ) const {

    o << "_DAGClass     = " << getDAGClass() << std::endl;
    o << "_distribution = " << distribution->briefInfo() << std::endl;
    o << "_touched      = " << ( touched ? Boolean( true ) : Boolean( false ) ) << std::endl;
    o << "_value        = NA" << std::endl;
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
void IntegratorNode::printValue( std::ostream& o ) const {

    o << "NA";
}


/** Complete info about object */
std::string IntegratorNode::richInfo(void) const {

    std::ostringstream o;
    o << "IntegratorNode:" << std::endl;
    o << "Clamped      = " << ( clamped ? "true" : "false" ) << std::endl;
    o << "Touched      = " << ( touched ? "true" : "false" ) << std::endl;
    o << "Distribution = ";
    distribution->printValue( o );
    o << std::endl;
    o << "CondLikes    = ";
    condLikes->printValue( o );
    o << std::endl;
    o << "Value        = NULL" << std::endl;
    o << "Stored value = NULL" << std::endl;

    return o.str();
}


/** Restore the old value of the node and tell affected */
void IntegratorNode::restore() {

    if ( touched ) {

        lnProb          = storedLnProb;
        storedLnProb    = 1.0E6;    // An almost impossible value for the density
    }

    touched = false;
 
    for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->restoreAffected();
}


/**
 * Set value: same as clamp, but do not clamp. This function will
 * also be used by moves to propose a new value.
 */
void IntegratorNode::setValue( RbObject* val, std::set<StochasticNode*>& affected ) {

    throw RbException( "Cannot set value of integrator node" );
}


/**
 * Swap parent node. Note that this function will be called by the tree plate
 * when the topology is changed. It is also used by the parser.
 * This function should be called from the mutateTo function, otherwise it
 * is dangerous because the distribution parameters will not be accommodated.
 */
void IntegratorNode::swapParentNode( DAGNode* oldNode, DAGNode* newNode ) {

    if ( parents.find( oldNode ) == parents.end() )
        throw RbException( "Node is not a parent" );

    oldNode->removeChildNode( this );
    newNode->addChildNode   ( this );
    parents.erase ( oldNode );
    parents.insert( newNode );

    if ( oldNode == condLikes )
        condLikes = static_cast<MemberNode*>( newNode );

    if ( !touched ) {

        storedLnProb = lnProb;
        touched = true;
    }

    for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->touchAffected();
}


/** Unclamp the value; use the clamped value as initial value */
void IntegratorNode::unclamp( void ) {

    throw RbException( "Cannot unclamp integrator node" );
}

