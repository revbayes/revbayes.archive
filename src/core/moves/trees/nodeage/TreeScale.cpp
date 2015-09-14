//
//  TreeScale.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 7/12/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "TreeScale.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

TreeScale::TreeScale(StochasticNode<TimeTree> *t, StochasticNode<double> *r, double d, bool tu, double w) : CompoundMove( std::vector<DagNode*>(), w, tu),
    tree( t ),
    rootAge( r ),
    delta( d )
{
    
    nodes.insert( tree );
    if ( rootAge != NULL )
    {
        nodes.insert( rootAge );
    }
    
}



/* Clone object */
TreeScale* TreeScale::clone( void ) const
{
    
    return new TreeScale( *this );
}



const std::string& TreeScale::getMoveName( void ) const
{
    static std::string name = "Tree-Scale";
    
    return name;
}


/** Perform the move */
double TreeScale::performCompoundMove( void )
{
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    TimeTree& tau = tree->getValue();
    
    TopologyNode& node = tau.getRoot();
    
    // we need to work with the times
    double my_age      = node.getAge();
    
    // now we store all necessary values
    storedAge = my_age;
    
    // draw new ages and compute the hastings ratio at the same time
    double u = rng->uniform01();
    double scalingFactor = std::exp( delta * ( u - 0.5 ) );
    
    // rescale the subtrees
    TreeUtilities::rescaleSubtree(&tau, &node, scalingFactor );
    
    if ( rootAge != NULL )
    {
        rootAge->setValue( new double(my_age * scalingFactor) );
    }
    
    // compute the Hastings ratio
    double lnHastingsratio = log( scalingFactor ) * tau.getNumberOfInteriorNodes();
    
    return lnHastingsratio;
}


void TreeScale::printParameterSummary(std::ostream &o) const
{

    o << "delta = " << delta;

}


void TreeScale::rejectCompoundMove( void )
{
    
    TimeTree& tau = tree->getValue();
    
    TopologyNode& node = tau.getRoot();
    
    // undo the proposal
    TreeUtilities::rescaleSubtree(&tau, &node, storedAge / node.getAge() );
    
    if ( rootAge != NULL )
    {
        rootAge->setValue( new double(storedAge) );
    }
    
    
#ifdef ASSERTIONS_TREE
    if ( fabs(storedAge - node.getAge()) > 1E-8 )
    {
        throw RbException("Error while rejecting SubtreeScale proposal: Node ages were not correctly restored!");
    }
#endif
    
}


void TreeScale::swapNode(DagNode *oldN, DagNode *newN)
{
    // call the parent method
    CompoundMove::swapNode(oldN, newN);
    
    if ( oldN == tree )
    {
        tree = static_cast<StochasticNode<TimeTree>* >(newN);
    }
    else if ( oldN == rootAge )
    {
        rootAge = static_cast<StochasticNode<double>* >(newN);
    }
    
}


void TreeScale::tune( void )
{
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.234 )
    {
        delta *= (1.0 + ((rate-0.234)/0.766) );
    }
    else
    {
        delta /= (2.0 - rate/0.234 );
    }
}

