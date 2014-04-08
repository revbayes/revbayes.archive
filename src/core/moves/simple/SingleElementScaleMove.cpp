#include "SingleElementScaleMove.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMathVector.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>

using namespace RevBayesCore;

/**
 * Simple constructor only setting the interval members from the given parameters.
 *
 * \param[in]    n    The stochastic node holding the vector of doubles.
 * \param[in]    l    The scaling parameter (lambda).
 * \param[in]    t    Should this move be tuned.
 * \param[in]    w    Weight of the proposal.
 */
SingleElementScaleMove::SingleElementScaleMove( const std::vector< StochasticNode< double > *> &n, double l, bool t, double w ) : Move( n[0], w, t ), changed(false), lambda( l ), storedValue( 0.0 ), variable( n ) {
    
    for (std::vector< StochasticNode<double> *>::const_iterator it = n.begin(); it != n.end(); it++)
        nodes.insert( *it );
}


/**
 * Accept the move and reset internal flags.
 *
 */
void SingleElementScaleMove::acceptMove( void ) {
    // nothing to do
    changed = false;
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of the move. 
 */
SingleElementScaleMove* SingleElementScaleMove::clone( void ) const {
    
    return new SingleElementScaleMove( *this );
}


/**
 * The function returns a string containing the name of the move. This name can be used for output purposes.
 *
 * \return The move's name. 
 */
const std::string& SingleElementScaleMove::getMoveName( void ) const {
    static std::string name = "SingleElementScale";
    
    return name;
}


/**
 * Perform the actual move, i.e., propose a new value.
 * First, we uniformly pick a new index, which is then stored for a potential reject call.
 * Additionally, we store the value of the chose element too in case of a reject call.
 * The, we draw randumly u ~ Uniform(0,1) and compute the scaling factor by
 * sf = exp( lambda * ( u - 0.5 ) )
 *
 * \return The Hastings ratio. 
 */
double SingleElementScaleMove::performMove( double& probRatio ) {
    
    // test wether a move has been performed without accept/reject in between
    if (changed) 
    {
        throw RbException("Trying to execute a simple moves twice without accept/reject in the meantime.");
    }
    changed = true;
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    // choose an index
    index = int(rng->uniform01() * variable.size());
    
    StochasticNode<double> *theNode = variable[index];
    
    // copy value
    storedValue = theNode->getValue();
    
    // Generate new value (no reflection, so we simply abort later if we propose value here outside of support)
    double u = rng->uniform01();
    double scalingFactor = std::exp( lambda * ( u - 0.5 ) );
    theNode->getValue() *= scalingFactor;
    
    // touch the node
    theNode->touch();
    
    // calculate the probability ratio for the node we just changed
    probRatio = theNode->getLnProbabilityRatio();
    
    if ( probRatio != RbConstants::Double::inf && probRatio != RbConstants::Double::neginf ) 
    {
        
        std::set<DagNode* > affectedNodes;
        theNode->getAffectedNodes(affectedNodes);
        for (std::set<DagNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); ++i) 
        {
            DagNode* theAffectedNode = *i;
            //std::cout << theAffectedNode->getName() << "  " << theAffectedNode->getLnProbabilityRatio() << "\n";
            probRatio += theAffectedNode->getLnProbabilityRatio();
        }
    }
    
    // compute the Hastings ratio
    double lnHastingsratio = log( scalingFactor );
    
    return lnHastingsratio;
}


void SingleElementScaleMove::printParameterSummary(std::ostream &o) const {
    o << "lambda = " << lambda;
}


void SingleElementScaleMove::rejectMove( void ) {
	
    changed = false;
    
    // swap current value and stored value
    variable[index]->getValue() = storedValue;
	
}


void SingleElementScaleMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    Move::swapNode(oldN, newN);
    
    
    for (size_t i = 0; i < variable.size(); ++i)
    {
        if ( variable[i] == oldN ) 
        {
            variable[i] = static_cast<StochasticNode<double> *>(newN);
            break;
        }
    }
    
}


void SingleElementScaleMove::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.44 ) {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else {
        lambda /= (2.0 - rate/0.44 );
    }
}

